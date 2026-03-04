// MinimapHUD.c
// Minimap HUD display — SCR_InfoDisplay'den türetilmiş temel sınıf.
// Workbench'te Prefabs/HUD/MinimapHUD.et prefabına bağlanır.

class MinimapHUD : SCR_InfoDisplay
{
	// Layout içindeki widget referansları
	protected ImageWidget   m_wMinimapImage;
	protected Widget        m_wPlayerIcon;
	protected TextWidget    m_wCompassText;

	//----------------------------------------------------------------------------------------------
	// Başlatma — layout yüklenince widget referanslarını al
	override event void OnStartDraw(IEntity owner)
	{
		super.OnStartDraw(owner);

		if (!m_wRoot)
			return;

		m_wMinimapImage  = ImageWidget.Cast(m_wRoot.FindAnyWidget("MinimapImage"));
		m_wPlayerIcon    = m_wRoot.FindAnyWidget("PlayerIcon");
		m_wCompassText   = TextWidget.Cast(m_wRoot.FindAnyWidget("CompassText"));
	}

	//----------------------------------------------------------------------------------------------
	// Her frame çağrılır — minimap verilerini güncelle
	override event void UpdateValues(IEntity owner, float timeSlice)
	{
		super.UpdateValues(owner, timeSlice);

		if (!m_wRoot || !m_wRoot.IsVisible())
			return;

		UpdateCompass(owner);
		UpdatePlayerIcon(owner);
	}

	//----------------------------------------------------------------------------------------------
	// Pusula açısını güncelle
	protected void UpdateCompass(IEntity owner)
	{
		if (!m_wCompassText || !owner)
			return;

		vector angles = owner.GetAngles();
		float heading = angles[0];
		if (heading < 0)
			heading += 360;

		m_wCompassText.SetText(string.Format("%1°", Math.Round(heading)));
	}

	//----------------------------------------------------------------------------------------------
	// Oyuncu ikonunu harita üzerinde konumlandır
	protected void UpdatePlayerIcon(IEntity owner)
	{
		if (!m_wPlayerIcon || !owner)
			return;

		// Oyuncu dünya konumunu al
		vector worldPos = owner.GetOrigin();

		// TODO: Dünya koordinatını minimap UV koordinatına dönüştür
		// float u = (worldPos[0] - mapOriginX) / mapSizeX;
		// float v = (worldPos[2] - mapOriginZ) / mapSizeZ;
		// FrameSlot.SetPos(m_wPlayerIcon, u * minimapWidth, v * minimapHeight);
	}

	//----------------------------------------------------------------------------------------------
	// HUD'u göster / gizle
	override void DisplayStartDraw(IEntity owner)
	{
		super.DisplayStartDraw(owner);
		Show(true, UIConstants.FADE_RATE_FAST);
	}

	override void DisplayStopDraw(IEntity owner)
	{
		super.DisplayStopDraw(owner);
		Show(false, UIConstants.FADE_RATE_FAST);
	}
}
