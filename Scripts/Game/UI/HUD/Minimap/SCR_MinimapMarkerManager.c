// SCR_MinimapMarkerManager.c
// Manages display of map markers (capture points, objectives, etc.) on the minimap
// Converts world positions to minimap-relative positions and renders marker icons

class SCR_MinimapMarkerManager
{
	// Container widget for marker icons
	protected Widget m_wMarkerContainer;

	// Configuration reference
	protected ref SCR_MinimapConfig m_Config;

	// Tracked marker widgets (keyed by marker ID)
	protected ref map<int, Widget> m_mMarkerWidgets = new map<int, Widget>();

	// Update interval tracking
	protected float m_fTimeSinceLastUpdate;
	static const float MARKER_UPDATE_INTERVAL = 0.5; // Update markers every 0.5 seconds

	//------------------------------------------------------------------------------------------------
	void Init(Widget container, SCR_MinimapConfig config)
	{
		m_wMarkerContainer = container;
		m_Config = config;
	}

	//------------------------------------------------------------------------------------------------
	void Cleanup()
	{
		// Remove all marker widgets
		if (m_mMarkerWidgets)
		{
			foreach (int id, Widget w : m_mMarkerWidgets)
			{
				if (w)
					w.RemoveFromHierarchy();
			}
			m_mMarkerWidgets.Clear();
		}
	}

	//------------------------------------------------------------------------------------------------
	void Update(IEntity player)
	{
		if (!m_wMarkerContainer || !m_Config || !m_Config.GetShowMarkers())
		{
			if (m_wMarkerContainer)
				m_wMarkerContainer.SetVisible(false);
			return;
		}

		m_wMarkerContainer.SetVisible(true);

		if (!player)
			return;

		// Throttle marker updates for performance
		m_fTimeSinceLastUpdate += System.GetFrameTimeS();
		if (m_fTimeSinceLastUpdate < MARKER_UPDATE_INTERVAL)
			return;
		m_fTimeSinceLastUpdate = 0;

		// Get the map entity for marker access
		SCR_MapEntity mapEntity = SCR_MapEntity.GetMapInstance();
		if (!mapEntity)
			return;

		vector playerPos = player.GetOrigin();
		float zoom = m_Config.GetZoomLevel();

		// The map widget system handles marker rendering natively
		// in MINIMAP mode, so we primarily ensure visibility is correct.
		// Additional custom markers can be managed here.

		UpdateCapturePointMarkers(mapEntity, playerPos, zoom);
	}

	//------------------------------------------------------------------------------------------------
	// Update capture point / objective markers on the minimap
	protected void UpdateCapturePointMarkers(SCR_MapEntity mapEntity, vector playerPos, float zoom)
	{
		if (!mapEntity)
			return;

		// The built-in map marker system handles rendering of:
		// - Capture points / flags
		// - Objectives
		// - Vehicle markers
		// - Player group markers
		// These are automatically displayed when the map is in MINIMAP mode
		// with the proper MapDescriptorProps configuration.

		// Custom marker logic can be added here for additional markers
		// that aren't part of the standard map system.
	}

	//------------------------------------------------------------------------------------------------
	// Convert a world position to minimap-relative screen position
	// Returns true if the position is within the minimap visible area
	static bool WorldToMinimapPos(vector worldPos, vector playerPos, float zoom, float minimapSize, out float outX, out float outY)
	{
		// Calculate offset from player
		float dx = worldPos[0] - playerPos[0];
		float dz = worldPos[2] - playerPos[2];

		// Scale based on zoom (higher zoom = more detail = smaller world area shown)
		float scale = zoom * (minimapSize / 1000.0);
		float screenX = dx * scale;
		float screenY = -dz * scale; // Invert Z because screen Y is top-down

		// Check if within minimap bounds (circular)
		float halfSize = minimapSize * 0.5;
		float distSq = screenX * screenX + screenY * screenY;
		float radiusSq = halfSize * halfSize;

		if (distSq > radiusSq)
			return false;

		// Convert to minimap-local coordinates (center = halfSize, halfSize)
		outX = halfSize + screenX;
		outY = halfSize + screenY;

		return true;
	}
}
