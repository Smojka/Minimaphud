// SCR_MinimapConfig.c
// Minimap configuration singleton - manages all minimap settings
// Persists via GameUserSettings so changes survive between sessions

class SCR_MinimapConfig
{
	private static ref SCR_MinimapConfig s_Instance;

	// Zoom settings
	protected float m_fZoomLevel = 0.5;
	static const float ZOOM_MIN = 0.1;
	static const float ZOOM_MAX = 5.0;
	static const float ZOOM_DEFAULT = 0.5;
	static const float ZOOM_STEP = 0.1;

	// Display settings
	protected float m_fMinimapSize = 256;
	protected float m_fOpacity = 0.85;
	protected bool m_bRotateWithPlayer = true;
	protected bool m_bShowMarkers = true;
	protected bool m_bEnabled = true;

	//------------------------------------------------------------------------------------------------
	static SCR_MinimapConfig GetInstance()
	{
		if (!s_Instance)
			s_Instance = new SCR_MinimapConfig();
		return s_Instance;
	}

	//------------------------------------------------------------------------------------------------
	float GetZoomLevel()
	{
		return m_fZoomLevel;
	}

	//------------------------------------------------------------------------------------------------
	void SetZoomLevel(float zoom)
	{
		m_fZoomLevel = Math.Clamp(zoom, ZOOM_MIN, ZOOM_MAX);
	}

	//------------------------------------------------------------------------------------------------
	void ZoomIn()
	{
		SetZoomLevel(m_fZoomLevel + ZOOM_STEP);
	}

	//------------------------------------------------------------------------------------------------
	void ZoomOut()
	{
		SetZoomLevel(m_fZoomLevel - ZOOM_STEP);
	}

	//------------------------------------------------------------------------------------------------
	float GetMinimapSize()
	{
		return m_fMinimapSize;
	}

	//------------------------------------------------------------------------------------------------
	void SetMinimapSize(float size)
	{
		m_fMinimapSize = Math.Clamp(size, 128, 512);
	}

	//------------------------------------------------------------------------------------------------
	float GetOpacity()
	{
		return m_fOpacity;
	}

	//------------------------------------------------------------------------------------------------
	void SetOpacity(float opacity)
	{
		m_fOpacity = Math.Clamp(opacity, 0.1, 1.0);
	}

	//------------------------------------------------------------------------------------------------
	bool GetRotateWithPlayer()
	{
		return m_bRotateWithPlayer;
	}

	//------------------------------------------------------------------------------------------------
	void SetRotateWithPlayer(bool rotate)
	{
		m_bRotateWithPlayer = rotate;
	}

	//------------------------------------------------------------------------------------------------
	bool GetShowMarkers()
	{
		return m_bShowMarkers;
	}

	//------------------------------------------------------------------------------------------------
	void SetShowMarkers(bool show)
	{
		m_bShowMarkers = show;
	}

	//------------------------------------------------------------------------------------------------
	bool IsEnabled()
	{
		return m_bEnabled;
	}

	//------------------------------------------------------------------------------------------------
	void SetEnabled(bool enabled)
	{
		m_bEnabled = enabled;
	}

	//------------------------------------------------------------------------------------------------
	// Load settings from the game user settings module
	void LoadSettings()
	{
		BaseContainer settings = GetGame().GetGameUserSettings().GetModule("SCR_MinimapSettingsModule");
		if (!settings)
			return;

		float zoom;
		if (settings.Get("m_fZoomLevel", zoom))
			m_fZoomLevel = Math.Clamp(zoom, ZOOM_MIN, ZOOM_MAX);

		float size;
		if (settings.Get("m_fMinimapSize", size))
			m_fMinimapSize = Math.Clamp(size, 128, 512);

		float opacity;
		if (settings.Get("m_fOpacity", opacity))
			m_fOpacity = Math.Clamp(opacity, 0.1, 1.0);

		bool rotate;
		if (settings.Get("m_bRotateWithPlayer", rotate))
			m_bRotateWithPlayer = rotate;

		bool markers;
		if (settings.Get("m_bShowMarkers", markers))
			m_bShowMarkers = markers;

		bool enabled;
		if (settings.Get("m_bEnabled", enabled))
			m_bEnabled = enabled;
	}

	//------------------------------------------------------------------------------------------------
	// Save settings to the game user settings module
	void SaveSettings()
	{
		BaseContainer settings = GetGame().GetGameUserSettings().GetModule("SCR_MinimapSettingsModule");
		if (!settings)
			return;

		settings.Set("m_fZoomLevel", m_fZoomLevel);
		settings.Set("m_fMinimapSize", m_fMinimapSize);
		settings.Set("m_fOpacity", m_fOpacity);
		settings.Set("m_bRotateWithPlayer", m_bRotateWithPlayer);
		settings.Set("m_bShowMarkers", m_bShowMarkers);
		settings.Set("m_bEnabled", m_bEnabled);

		GetGame().UserSettingsChanged();
	}
}
