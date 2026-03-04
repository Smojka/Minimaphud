// SCR_MinimapDisplay.c
// Main circular minimap HUD display
// Features:
//   - Circular minimap at bottom-center of screen
//   - Map texture with roads, terrain, lakes visible
//   - Rotates with player heading (GTA-style)
//   - Player icon at center
//   - Tracks player position in real-time
//   - Adjustable zoom level
//   - Shows map markers (capture points, objectives)

class SCR_MinimapDisplay : SCR_InfoDisplay
{
	// Static reference for global access
	static SCR_MinimapDisplay s_Instance;

	// Configuration
	protected ref SCR_MinimapConfig m_Config;

	// Core widgets
	protected Widget m_wMinimapFrame;
	protected Widget m_wMapContainer;
	protected Widget m_wMapWidget;
	protected ImageWidget m_wCircleMask;
	protected ImageWidget m_wPlayerIcon;
	protected ImageWidget m_wCircleBorder;
	protected Widget m_wMarkerOverlay;

	// Map state
	protected MapConfiguration m_MapConfig;
	protected bool m_bMapInitialized;
	protected bool m_bFullMapOpen;
	protected float m_fLastUpdateTime;

	// Cached references
	protected MenuManager m_MenuManager;

	// Marker manager
	protected ref SCR_MinimapMarkerManager m_MarkerManager;

	// Map last zoom/pan state (for restoring after fullscreen map)
	protected float m_fSavedFullMapZoom = -1;
	protected vector m_vSavedFullMapPan;

	// Layout path for fallback
	protected const ResourceName MINIMAP_LAYOUT = "{DE15F59659B8CF68}UI/Layouts/HUD/MinimapHUD.layout";

	//------------------------------------------------------------------------------------------------
	override bool DisplayStartDrawInit(IEntity owner)
	{
		m_Config = SCR_MinimapConfig.GetInstance();
		m_Config.LoadSettings();

		m_MenuManager = GetGame().GetMenuManager();
		if (!m_MenuManager)
			return false;

		if (m_LayoutPath == string.Empty)
			m_LayoutPath = MINIMAP_LAYOUT;

		return true;
	}

	//------------------------------------------------------------------------------------------------
	override void DisplayStartDraw(IEntity owner)
	{
		s_Instance = this;

		if (!m_wRoot)
			return;

		// Find widgets from the layout hierarchy
		m_wMinimapFrame = m_wRoot.FindAnyWidget("MinimapFrame");
		m_wMapContainer = m_wRoot.FindAnyWidget("MapContainer");
		m_wCircleMask = ImageWidget.Cast(m_wRoot.FindAnyWidget("CircleMask"));
		m_wPlayerIcon = ImageWidget.Cast(m_wRoot.FindAnyWidget("PlayerIcon"));
		m_wCircleBorder = ImageWidget.Cast(m_wRoot.FindAnyWidget("CircleBorder"));
		m_wMarkerOverlay = m_wRoot.FindAnyWidget("MarkerOverlay");

		// Initialize marker manager
		if (m_wMarkerOverlay)
		{
			m_MarkerManager = new SCR_MinimapMarkerManager();
			m_MarkerManager.Init(m_wMarkerOverlay, m_Config);
		}

		// Register for fullscreen map open/close events
		if (SCR_MapEntity.GetOnMapOpen())
			SCR_MapEntity.GetOnMapOpen().Insert(OnFullMapOpened);
		if (SCR_MapEntity.GetOnMapClose())
			SCR_MapEntity.GetOnMapClose().Insert(OnFullMapClosed);

		// Register settings change listener
		GetGame().OnUserSettingsChangedInvoker().Insert(OnSettingsChanged);

		// Initialize the minimap with delay to ensure everything is loaded
		GetGame().GetCallqueue().CallLater(InitMinimap, 500, false);
	}

	//------------------------------------------------------------------------------------------------
	override void DisplayStopDraw(IEntity owner)
	{
		CloseMinimap();

		if (SCR_MapEntity.GetOnMapOpen())
			SCR_MapEntity.GetOnMapOpen().Remove(OnFullMapOpened);
		if (SCR_MapEntity.GetOnMapClose())
			SCR_MapEntity.GetOnMapClose().Remove(OnFullMapClosed);

		GetGame().OnUserSettingsChangedInvoker().Remove(OnSettingsChanged);

		GetGame().GetCallqueue().Remove(InitMinimap);
		GetGame().GetCallqueue().Remove(MinimapUpdateLoop);

		if (m_MarkerManager)
			m_MarkerManager.Cleanup();

		s_Instance = null;
		m_wRoot = null;
	}

	//------------------------------------------------------------------------------------------------
	override void DisplayUpdate(IEntity owner, float timeSlice)
	{
		if (!m_Config || !m_Config.IsEnabled())
		{
			if (m_wMinimapFrame)
				m_wMinimapFrame.SetVisible(false);
			return;
		}

		if (m_wMinimapFrame)
			m_wMinimapFrame.SetVisible(!m_bFullMapOpen);

		// Visual updates (rotation, opacity)
		UpdateVisuals(owner, timeSlice);
	}

	//------------------------------------------------------------------------------------------------
	// Initialize map in MINIMAP mode using SCR_MapEntity
	protected void InitMinimap()
	{
		if (m_bMapInitialized)
			return;

		IEntity player = SCR_PlayerController.GetLocalControlledEntity();
		if (!player)
		{
			GetGame().GetCallqueue().CallLater(InitMinimap, 500, false);
			return;
		}

		SCR_MapEntity mapEntity = SCR_MapEntity.GetMapInstance();
		if (!mapEntity)
		{
			GetGame().GetCallqueue().CallLater(InitMinimap, 500, false);
			return;
		}

		if (!m_wMapContainer)
			return;

		// Check if fullscreen map is already open - wait for it to close
		if (m_MenuManager)
		{
			SCR_MapMenuUI mapMenu = SCR_MapMenuUI.Cast(m_MenuManager.FindMenuByPreset(ChimeraMenuPreset.MapMenu));
			if (mapMenu)
			{
				GetGame().GetCallqueue().CallLater(InitMinimap, 500, false);
				return;
			}
		}

		// Set up map configuration in MINIMAP mode
		// This opens the map with proper texture layers including satellite imagery
		m_MapConfig = mapEntity.SetupMapConfig(EMapEntityMode.MINIMAP, "", m_wMapContainer);
		mapEntity.OpenMap(m_MapConfig);

		// Assign the MapWidget for rendering
		if (m_wMapWidget || m_wRoot)
		{
			Widget mapWidget = m_wRoot.FindAnyWidget("MapWidget");
			if (mapWidget)
				mapEntity.SetMapWidget(mapWidget);
		}

		// Re-enable character camera rendering (opening map disables it)
		PlayerController pc = GetGame().GetPlayerController();
		if (pc)
			pc.SetCharacterCameraRenderActive(true);

		m_bMapInitialized = true;

		// Start the continuous update loop
		GetGame().GetCallqueue().Remove(MinimapUpdateLoop);
		GetGame().GetCallqueue().CallLater(MinimapUpdateLoop, 1, false);
	}

	//------------------------------------------------------------------------------------------------
	// Close the minimap and release map resources
	protected void CloseMinimap()
	{
		GetGame().GetCallqueue().Remove(MinimapUpdateLoop);

		SCR_MapEntity mapEntity = SCR_MapEntity.GetMapInstance();
		if (mapEntity && m_bMapInitialized && mapEntity.IsOpen())
		{
			mapEntity.CloseMap();
		}

		m_bMapInitialized = false;
		m_MapConfig = null;
	}

	//------------------------------------------------------------------------------------------------
	// Continuous map pan/zoom update loop
	protected void MinimapUpdateLoop()
	{
		if (!m_bMapInitialized || !m_Config || !m_Config.IsEnabled())
			return;

		// Get player entity
		IEntity player = SCR_PlayerController.GetLocalControlledEntity();
		if (!player)
		{
			ScheduleNextUpdate(100);
			return;
		}

		// Get map entity
		SCR_MapEntity mapEntity = SCR_MapEntity.GetMapInstance();
		if (!mapEntity || !mapEntity.IsOpen())
		{
			ScheduleNextUpdate(100);
			return;
		}

		// Skip updates when fullscreen map or any menu is open
		if (m_bFullMapOpen)
		{
			ScheduleNextUpdate(100);
			return;
		}

		if (m_MenuManager && m_MenuManager.IsAnyMenuOpen())
		{
			ScheduleNextUpdate(100);
			return;
		}

		// Get player world position
		vector playerPos = player.GetOrigin();
		float worldX = playerPos[0];
		float worldZ = playerPos[2];

		// Apply zoom level from config
		float zoom = m_Config.GetZoomLevel();
		mapEntity.SetZoom(zoom, true);

		// Convert world position to screen coordinates for panning
		float screenX, screenY;
		mapEntity.WorldToScreen(worldX, worldZ, screenX, screenY);

		WorkspaceWidget workspace = GetGame().GetWorkspace();
		if (workspace)
		{
			screenX = workspace.DPIUnscale(screenX);
			screenY = workspace.DPIUnscale(screenY);
		}

		// Center map on player position
		mapEntity.SetPan(screenX, screenY, true, true);

		// Ensure camera stays active
		PlayerController pc = GetGame().GetPlayerController();
		if (pc)
			pc.SetCharacterCameraRenderActive(true);

		// Schedule next update
		ScheduleNextUpdate(1);
	}

	//------------------------------------------------------------------------------------------------
	protected void ScheduleNextUpdate(int delayMs)
	{
		GetGame().GetCallqueue().Remove(MinimapUpdateLoop);
		GetGame().GetCallqueue().CallLater(MinimapUpdateLoop, delayMs, false);
	}

	//------------------------------------------------------------------------------------------------
	// Per-frame visual updates: rotation, opacity, player icon
	protected void UpdateVisuals(IEntity owner, float timeSlice)
	{
		IEntity player = SCR_PlayerController.GetLocalControlledEntity();
		if (!player)
			return;

		// Rotate map with player heading (GTA-style)
		if (m_Config.GetRotateWithPlayer() && m_wMapContainer)
		{
			vector angles = player.GetAngles();
			float heading = angles[0]; // Yaw = heading direction
			m_wMapContainer.SetRotation(heading);
		}
		else if (m_wMapContainer)
		{
			m_wMapContainer.SetRotation(0);
		}

		// Player icon counter-rotation (stays pointing forward/north)
		if (m_wPlayerIcon && m_Config.GetRotateWithPlayer())
		{
			// Player icon doesn't need counter-rotation
			// It represents current position, always at center
			// Arrow direction is inherently "up" = forward
		}

		// Apply opacity
		if (m_wMinimapFrame)
			m_wMinimapFrame.SetOpacity(m_Config.GetOpacity());

		// Update markers on the minimap
		if (m_MarkerManager && m_Config.GetShowMarkers())
			m_MarkerManager.Update(player);
	}

	//------------------------------------------------------------------------------------------------
	// Called when the fullscreen map is opened
	protected void OnFullMapOpened(MapConfiguration config)
	{
		if (config.MapEntityMode != EMapEntityMode.FULLSCREEN)
			return;

		m_bFullMapOpen = true;

		// Save current fullscreen map state
		SCR_MapEntity mapEntity = SCR_MapEntity.GetMapInstance();
		if (mapEntity)
		{
			m_fSavedFullMapZoom = mapEntity.GetCurrentZoom();
			m_vSavedFullMapPan = mapEntity.GetCurrentPan();
		}

		if (m_wMinimapFrame)
			m_wMinimapFrame.SetVisible(false);
	}

	//------------------------------------------------------------------------------------------------
	// Called when the fullscreen map is closed
	protected void OnFullMapClosed(MapConfiguration config)
	{
		if (config.MapEntityMode != EMapEntityMode.FULLSCREEN)
			return;

		m_bFullMapOpen = false;

		// Re-initialize minimap after fullscreen map closes
		m_bMapInitialized = false;
		GetGame().GetCallqueue().CallLater(InitMinimap, 200, false);
	}

	//------------------------------------------------------------------------------------------------
	protected void OnSettingsChanged()
	{
		m_Config.LoadSettings();
	}

	//------------------------------------------------------------------------------------------------
	// Static getter for external access
	static SCR_MinimapDisplay GetInstance()
	{
		return s_Instance;
	}
}
