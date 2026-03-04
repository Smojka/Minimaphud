# Arma Reforger Tools – Workbench Araştırması

Bu doküman, **Minimaphud** modu geliştirmek için ihtiyaç duyulan Arma Reforger Workbench araçları, dosya formatları, iş akışları ve UI/HUD geliştirme hakkında kapsamlı bilgi içermektedir.

---

## 1. Workbench Nedir?

**Enfusion Workbench**, Arma Reforger'ın resmi modlama aracıdır. Steam'deki "Tools" kategorisinde ücretsiz olarak bulunur. İçinde birden fazla editör barındırır ve bir "hepsi bir arada" geliştirme ortamı sunar.

**Kurulum:**
1. Steam → Library → Tools → "Arma Reforger Tools" kur.
2. Workbench'i ilk açtığında, oyun verisinin konumunu ve proje klasörünü belirtirsin.
3. Mod çalışma alanı genellikle şu konumdadır:  
   `Documents\My Games\ArmaReforgerWorkbench\addons\<ModAdin>`

---

## 2. Workbench Editörleri / Modülleri

| Editör | Amaç |
|---|---|
| **World Editor** | Harita/terrain ve dünya objeleri oluşturma/düzenleme |
| **Script Editor** | Oyun lojiği ve davranış scriptleri (Enforce Script / `.c` dosyaları) |
| **Layout Editor** | UI/HUD tasarımı (`.layout` dosyaları) |
| **Resource Manager** | Asset, prefab ve kaynak yönetimi |
| **Particle Editor** | Parçacık efektleri |
| **Animation Editor** | Animasyon oluşturma ve düzenleme |
| **Audio Editor** | Ses dosyaları ve banka yönetimi |
| **Behavior Editor** | Yapay zeka davranış ağaçları |
| **Config Editor** | `.conf` yapılandırma dosyaları düzenleme |

### Workbench Pencere Düzeni (Layout) Özelleştirme
- Pencereleri "Windows" menüsünden aç/kapat.
- Panelleri sürükle-bırak ile istediğin yere yerleştir (dock/undock).
- Eklenti (plugin) sistemi sayesinde kendi araç panellerini ekleyebilirsin.

---

## 3. Dosya Formatları (File Types)

### `.et` – Entity Template (Varlık Şablonu)
- Enfusion motorunun **prefab sistemi**dir; en temel dosya türüdür.
- Bir `.et` dosyası; bir varlığın (entity) hiyerarşisini, bileşenlerini (components) ve özelliklerini saklar.
- **Özellikler:**
  - Prefablar birbirini referans edebilir (recursive/iç içe yapı).
  - Miras (inheritance) desteği vardır: Bir `.et` dosyasını temel alarak parametrelerini override ederek yeni varlıklar yaratabilirsin.
  - Tüm instance'lar prefab'tan türer; prefab güncellenmesi tüm instance'ları etkiler.
  - Doğrudan text editörle değil, Workbench editörleriyle açılır/düzenlenir.
- **Kullanım Alanları:**
  - Araç (jeep, tank) şablonu
  - Asker varlığı ve fraksiyona özgü alt şablonlar
  - HUD bileşenleri içeren oyuncu kontrolcüsü şablonu (`DefaultPlayerController.et`)
  - Karmaşık prop veya bina montajları
- **Nasıl oluşturulur:** World Editor'da entity oluştur → hiyerarşide sağ tıkla → "Create Prefab From Selection."
- **Nasıl düzenlenir:** `.et` dosyasına çift tıkla → "Prefab Edit Mode" ile editörde açılır.

### `.ct` – Component Template (Bileşen Şablonu)
- Tek bir bileşeni (physics, AI, render, vb.) tanımlayan şablon dosyasıdır.

### `.conf` – Config (Yapılandırma) Dosyası
- Oyun sistemleri, silah verileri, senaryo lojiği gibi hiyerarşik yapılandırmaları saklar.
- Config Editor ile düzenlenir.
- Miras desteği vardır; mevcut config'i temel alarak variant oluşturabilirsin.

### `.layout` – UI Yerleşim Dosyası
- Butonu, paneli, ikonu, metni vb. UI elementlerinin yapısını ve hiyerarşisini tanımlar.
- Layout Editor ile görsel olarak tasarlanır.
- Script tarafından widget isimleriyle referans edilir (`FindAnyWidget("WidgetName")`).

### `.c` – Enforce Script Dosyası
- C'ye benzer sözdizimi olan Enfusion'un script dilidir.
- Dosyalar `Scripts/` klasörü altında saklanır; Script Editor'da yazılır ve debug edilir.
- Script klasörü yapısı önemlidir (aşağıya bak).

### `.xob` – 3D Model Dosyası
- Enfusion'a özgü 3D model formatıdır.
- Geometri, materyal referansları ve iskelet bilgilerini içerir.
- Dışarıdan içe aktararak (import) oluşturulur; Resource Browser'dan yönetilir.

### `.edds` – Texture (Doku) Dosyası
- Enfusion'a özgü DDS doku formatıdır (`.dds`'in optimize edilmiş versiyonu).
- PNG/JPG gibi formatlar, Workbench'in import araçlarıyla `.edds`'e dönüştürülür.
- Sıkıştırma (compression) ve renk uzayı (color space) ayarları yapılabilir.

### `.meta` – Meta Dosyası
- Her asset'e eşlik eden, o asset'in **GUID** (Globally Unique Identifier) bilgisini içerir.
- Motor, dosya yolları yerine GUID ile asset bağlantılarını takip eder.
- Bu sayede modlar assetleri override edebilir veya değiştirebilir.

---

## 4. Resource Manager (Kaynak Yöneticisi)

- Tüm proje ve oyun assetlerine göz atılan, arama yapılan merkezi paneldir.
- Sol tarafta klasör/dizin ağacı, sağda içerik listelenir.
- Hem yerel hem de paketlenmiş (game) dosyalara göz atılabilir.
- **Context Actions (Sağ Tık Menüsü):**
  - Import (içe aktar)
  - Create (yeni oluştur)
  - Override (override et)
  - Duplicate (kopyala)
  - Inherit (miras al)

---

## 5. Script Klasörü Yapısı

Script dosyaları hangi klasörde olduğuna göre farklı modüllere yüklenir:

| Klasör | Modül | Açıklama |
|---|---|---|
| `Scripts/Game/` | Game | Oyun lojiği scriptleri |
| `Scripts/GameLib/` | GameLib | Kütüphane/yardımcı scriptler |
| `Scripts/WorkbenchGame/` | WorkbenchGame | Workbench plugin scriptleri |

> **Önemli:** Script dosyaları bu klasörlerin dışına konulursa motor tarafından **yüklenmez/görmezden gelinir**.

---

## 6. Workbench Plugin Sistemi

Workbench'e özel araçlar eklentiler (plugin) aracılığıyla yazılabilir:

- Plugin dosyaları `Scripts/WorkbenchGame/` altına konulur.
- Dosya adı `[ClassName]Plugin.c` formatında olmalıdır.
- Sınıf `WorkbenchPlugin`'den türetilmelidir.
- `[WorkbenchPluginAttribute]` attribute'u ile isim, kısayol ve modül tanımlanır.
- Workbench → Plugins menüsünden çalıştırılır.

**Basit Plugin Örneği:**
```c
[WorkbenchPluginAttribute("Prefab Hierarchy Helper", "Örnek plugin")]
class MyPrefabHierarchyPlugin : WorkbenchPlugin
{
    override void Run()
    {
        WorldEditor worldEditor = Workbench.GetModule(WorldEditor);
        IEntity prefabEntity = worldEditor.GetSelectedEntity();
        if (!prefabEntity)
        {
            Print("Hiç entity seçilmedi.");
            return;
        }

        array<IComponent> components = {};
        prefabEntity.GetComponents(components);

        foreach (IComponent comp : components)
        {
            PrintFormat("Bileşen: %1", comp.ToString());
        }
    }
}
```

---

## 7. UI / HUD Geliştirme (Minimap İçin)

Bu bölüm, **Minimaphud** modu için en kritik kısımdır.

### 7.1 Layout Dosyası Oluşturma

1. Layout Editor'ı Workbench'ten aç.
2. Yeni bir `.layout` dosyası oluştur (genellikle `UI/layouts/` klasörüne koy).
3. Widget hiyerarşini oluştur:
   - `FrameWidget` veya `OverlayWidget` → dış kapsayıcı
   - `ImageWidget` → minimap arka planı veya görüntü
   - `ImageWidget` → oyuncu/nesne ikonları (dinamik konumlandırma için)
   - `TextWidget` → isteğe bağlı etiketler
4. Her widget'a **benzersiz bir isim** ver (script tarafından bu isimle bulunur).

### 7.2 SCR_InfoDisplay ile HUD Scripti

Minimap gibi bilgi göstergeleri için `SCR_InfoDisplay` sınıfından türetme yapılır:

```c
class MyMinimapDisplay : SCR_InfoDisplay
{
    private ImageWidget m_MinimapImage;
    private ImageWidget m_PlayerIcon;

    override event void OnStartDraw(IEntity owner)
    {
        super.OnStartDraw(owner);

        // Widget isimlerini layout dosyasındaki isimlerle eşleştir
        if (!m_MinimapImage)
            m_MinimapImage = ImageWidget.Cast(m_wRoot.FindAnyWidget("MinimapImage"));
        if (!m_PlayerIcon)
            m_PlayerIcon = ImageWidget.Cast(m_wRoot.FindAnyWidget("PlayerIcon"));
    }

    override event void OnUpdateDraw(IEntity owner)
    {
        super.OnUpdateDraw(owner);
        // Burada oyuncu pozisyonunu al ve icon'u güncelle
    }
}
```

### 7.3 HUD'u Oyuncuya Bağlama

HUD'un oyunda görünmesi için oyuncu controller prefab'ını override etmen gerekir:

1. Resource Browser'dan `DefaultPlayerController.et` dosyasını bul.
2. Sağ tıkla → **Override** ile kendi mod klasörüne kopyala.
3. Override edilen prefab'ı aç → HUD bileşenleri listesine kendi `SCR_InfoDisplay` türevini ekle.

### 7.4 UI Geliştirme İpuçları

- HUD widget'ları kullanıcı etkileşimi (tıklama) almaz; yalnızca görüntüleme içindir.
- Widget'ları dinamik olarak hareket ettirmek için `SetPos()` veya transform property'lerini kullan.
- Minimap döndürme/zoom için widget transform özelliklerini çalışma zamanında güncelle.
- UI bağlamları (context) için `UIManagerComponent` kullanarak çoklu ekran/menü yönetimi yapabilirsin.

---

## 8. Proje Klasör Yapısı (Önerilen)

```
<ModAdin>/
├── Scripts/
│   ├── Game/
│   │   └── UI/
│   │       └── MinimapDisplay.c       ← SCR_InfoDisplay türevi
│   └── WorkbenchGame/
│       └── SamplePlugins/
│           └── MinimapHelperPlugin.c  ← Workbench plugin
├── UI/
│   └── layouts/
│       └── Minimap.layout             ← Layout dosyası
├── Prefabs/
│   └── Player/
│       └── DefaultPlayerController.et ← Override edilmiş prefab
└── mod.json                            ← Mod meta verisi
```

---

## 9. Temel İş Akışı Özeti

1. **Kurulum:** Steam'den Arma Reforger Tools kur → Workbench'i başlat → Proje oluştur.
2. **Layout Tasarımı:** Layout Editor → yeni `.layout` → widget'ları ekle ve isimlendir.
3. **Script Yazma:** Script Editor → `SCR_InfoDisplay`'den türet → widget'lara `FindAnyWidget` ile eriş.
4. **Prefab Override:** `DefaultPlayerController.et` dosyasını override et → HUD bileşenini ekle.
5. **Test:** Workbench'teki play mode ile test et → hata ayıkla.
6. **Paketleme:** Mod tamamlandığında Workshop için paketleyip yayınla.

---

## 10. Faydalı Kaynaklar

| Kaynak | URL |
|---|---|
| Bohemia Interactive Community Wiki | https://community.bistudio.com/wiki/Arma_Reforger |
| Mod Project Setup | https://community.bistudio.com/wiki/Arma_Reforger:Mod_Project_Setup |
| Data Modding Basics | https://community.bistudio.com/wiki/Arma_Reforger:Data_Modding_Basics |
| Prefabs Basics | https://community.bistudio.com/wiki/Arma_Reforger:Prefabs_Basics |
| Layout Creation | https://community.bistudio.com/wiki/Arma_Reforger:Layout_Creation |
| Resource Manager | https://community.bistudio.com/wiki/Arma_Reforger:Resource_Manager |
| Workbench Plugin Tutorial | https://community.bistudio.com/wiki/Arma_Reforger:Workbench_Plugin_Tutorial |
| Scripting Modding | https://community.bistudio.com/wiki/Arma_Reforger:Scripting_Modding |
| File Types | https://community.bistudio.com/wiki/Arma_Reforger:File_Types |
| Modding Boot Camp #4 – UI/HUD | https://reforger.armaplatform.com/news/modding-boot-camp-4-user-interface-and-hud |
| Arma Reforger Dev Hub | https://reforger.armaplatform.com/dev-hub |
| Arma Reforger Samples (GitHub) | https://github.com/BohemiaInteractive/Arma-Reforger-Samples |
| Workbench Video Tutorials | https://www.youtube.com/playlist?list=PLhFQ1b8sqM94_FlgXUWBNz-HUXgab6DuP |
| HUD/UI Addon Rehberi (Steam) | https://steamcommunity.com/sharedfiles/filedetails/?id=2814814735 |
| Armed Assault Wiki – Enfusion Workbench | https://armedassault.fandom.com/wiki/Enfusion_workbench |
