// SCR_MinimapSettings.c
// User settings module for the minimap
// Integrates with the game's pause menu settings system
// Allows players to adjust zoom, opacity, rotation, and other minimap options

// Settings module that registers with the game settings system
[BaseContainerProps(), SCR_BaseContainerLocalizedTitleField("m_sModuleName")]
class SCR_MinimapSettingsModule : SCR_GameplaySettingsModule
{
	[Attribute("0.5", UIWidgets.Slider, desc: "Minimap Zoom Level", params: "0.1 5.0 0.1")]
	float m_fZoomLevel;

	[Attribute("256", UIWidgets.Slider, desc: "Minimap Size (pixels)", params: "128 512 16")]
	float m_fMinimapSize;

	[Attribute("0.85", UIWidgets.Slider, desc: "Minimap Opacity", params: "0.1 1.0 0.05")]
	float m_fOpacity;

	[Attribute("1", UIWidgets.CheckBox, desc: "Rotate Map With Player")]
	bool m_bRotateWithPlayer;

	[Attribute("1", UIWidgets.CheckBox, desc: "Show Map Markers")]
	bool m_bShowMarkers;

	[Attribute("1", UIWidgets.CheckBox, desc: "Enable Minimap")]
	bool m_bEnabled;
}

// Modded pause menu to add minimap settings tab
modded class SCR_PauseMenuUI : ChimeraMenuBase
{
	//------------------------------------------------------------------------------------------------
	override void OnMenuOpen()
	{
		super.OnMenuOpen();

		// Minimap settings are accessible through the game settings
		// which already shows registered SCR_GameplaySettingsModule modules.
		// The SCR_MinimapSettingsModule above registers automatically.
	}
}

// Keybind actions for minimap zoom control during gameplay
class SCR_MinimapInputActions
{
	static const string ACTION_ZOOM_IN = "MinimapZoomIn";
	static const string ACTION_ZOOM_OUT = "MinimapZoomOut";
	static const string ACTION_TOGGLE = "MinimapToggle";

	//------------------------------------------------------------------------------------------------
	static void RegisterActions()
	{
		InputManager inputManager = GetGame().GetInputManager();
		if (!inputManager)
			return;

		inputManager.AddActionListener(ACTION_ZOOM_IN, EActionTrigger.PRESSED, OnZoomIn);
		inputManager.AddActionListener(ACTION_ZOOM_OUT, EActionTrigger.PRESSED, OnZoomOut);
		inputManager.AddActionListener(ACTION_TOGGLE, EActionTrigger.DOWN, OnToggle);
	}

	//------------------------------------------------------------------------------------------------
	static void UnregisterActions()
	{
		InputManager inputManager = GetGame().GetInputManager();
		if (!inputManager)
			return;

		inputManager.RemoveActionListener(ACTION_ZOOM_IN, EActionTrigger.PRESSED, OnZoomIn);
		inputManager.RemoveActionListener(ACTION_ZOOM_OUT, EActionTrigger.PRESSED, OnZoomOut);
		inputManager.RemoveActionListener(ACTION_TOGGLE, EActionTrigger.DOWN, OnToggle);
	}

	//------------------------------------------------------------------------------------------------
	protected static void OnZoomIn(float value, EActionTrigger trigger)
	{
		SCR_MinimapConfig config = SCR_MinimapConfig.GetInstance();
		if (config)
			config.ZoomIn();
	}

	//------------------------------------------------------------------------------------------------
	protected static void OnZoomOut(float value, EActionTrigger trigger)
	{
		SCR_MinimapConfig config = SCR_MinimapConfig.GetInstance();
		if (config)
			config.ZoomOut();
	}

	//------------------------------------------------------------------------------------------------
	protected static void OnToggle(float value, EActionTrigger trigger)
	{
		SCR_MinimapConfig config = SCR_MinimapConfig.GetInstance();
		if (config)
			config.SetEnabled(!config.IsEnabled());
	}
}
