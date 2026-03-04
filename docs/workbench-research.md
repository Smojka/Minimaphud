# Arma Reforger Tools – Workbench Derinlemesine Araştırması

Bu belgede, **Minimaphud** modu geliştirmek için Arma Reforger Workbench araçları, Enfusion motorunun çalışma mantığı, tüm dosya formatları ve gerçek kod örnekleri hakkında kapsamlı ve doğrulanmış bilgi içermektedir.

> **Not:** Bu dokümandaki tüm bilgiler, Bohemia Interactive'in resmi GitHub örnek projesinden (`BohemiaInteractive/Arma-Reforger-Samples`) bizzat okunan kaynak dosyalarına ve resmi wiki'ye dayanmaktadır. Üretilen kod örnekleri gerçek örneklerden türetilmiştir.

---

## İçindekiler

1. [Workbench Nedir?](#1-workbench-nedir)
2. [Enfusion Motorunun Çalışma Mantığı (ECS)](#2-enfusion-motorunun-çalışma-mantığı-ecs)
3. [Proje Dosyası: `.gproj`](#3-proje-dosyası-gproj)
4. [Dosya Formatları – Tam Liste](#4-dosya-formatları--tam-liste)
5. [Dünya Sistemi: `.ent`, `.layer`, SubScene](#5-dünya-sistemi-ent-layer-subscene)
6. [Prefab Sistemi: `.et` ve `.ct`](#6-prefab-sistemi-et-ve-ct)
7. [Konfigürasyon Sistemi: `.conf`](#7-konfigürasyon-sistemi-conf)
8. [Script Sistemi: Enforce Script (`.c`)](#8-script-sistemi-enforce-script-c)
9. [UI Sistemi: Layout ve Widget'lar](#9-ui-sistemi-layout-ve-widgetlar)
10. [Workbench Modülleri ve Plugin Sistemi](#10-workbench-modülleri-ve-plugin-sistemi)
11. [Mod Proje Klasör Yapısı (Gerçek Örnek)](#11-mod-proje-klasör-yapısı-gerçek-örnek)
12. [HUD/Minimap Geliştirme İş Akışı](#12-hudminimap-geliştirme-iş-akışı)
13. [Sık Yapılan Hatalar ve Dikkat Edilecekler](#13-sık-yapılan-hatalar-ve-dikkat-edilecekler)
14. [Faydalı Kaynaklar](#14-faydalı-kaynaklar)

---

## 1. Workbench Nedir?

**Enfusion Workbench**, Arma Reforger'ın resmi modlama ve içerik geliştirme ortamıdır. Steam'deki "Tools" kategorisinde **ücretsiz** olarak bulunur. Tek bir uygulama içinde birden fazla editör/modül barındırır.

**Kurulum:**
1. Steam → Library → Tools → "Arma Reforger Tools" kur.
2. Workbench'i ilk açışta yapılandırma sihirbazı çalışır; oyun veri yolunu ve çalışma alanını belirtirsin.
3. Proje (mod) klasörü tipik olarak şu konumdadır:  
   `Documents\My Games\ArmaReforgerWorkbench\addons\<ModAdin>`

---

## 2. Enfusion Motorunun Çalışma Mantığı (ECS)

Enfusion, **Entity-Component System (ECS)** mimarisine dayanır. Bu mimari şu prensiple çalışır:

- **Entity (Varlık):** Dünyada var olan her nesne bir Entity'dir. `IEntity` arayüzünü uygular.
- **Component (Bileşen):** Entity'lere eklenen, belirli bir işlevi yerine getiren parçalardır. `ScriptComponent`'dan türetilir.
- **System:** Belirli bileşen türlerine sahip entity'leri toplu olarak işleyen mantık katmanıdır.

### Temel Sınıflar

| Sınıf | Açıklama |
|---|---|
| `IEntity` | Tüm dünya nesnelerinin temel arayüzü. Konum, döndürme, ölçek sorguları, çocuk entity yönetimi. |
| `ScriptComponent` | Gameplay bileşenlerinin temel sınıfı. Entity'ye eklenir. |
| `BaseWorld` | Dünya bağlamına erişim. Entity sorguları, global durum. |
| `ScriptGame` | Üst düzey oyun mantığının giriş noktası. |

### Entity/Component Yaşam Döngüsü

```
Constructor → OnPostInit → EOnInit
    → [EOnFrame / EOnFixedFrame / EOnSimulate] (döngü)
        → OnDelete → Destructor
```

| Callback | Ne Zaman Çağrılır |
|---|---|
| `OnPostInit()` | Bileşen başlatıldığında |
| `EOnFrame(float timeSlice)` | Her render frame'inde |
| `EOnFixedFrame(float timeSlice)` | Sabit zaman adımında (fizik lojiği için) |
| `EOnSimulate(float timeSlice)` | Simülasyon tick'inde |
| `OnDelete()` | Entity/bileşen silinirken |

> **Önemli:** Bileşen yıkıcısında (`OnDelete`) üst entity'ye **referans verme**; bu entity o anda zaten silinmiş olabilir ve erişim hatalara (crash) neden olur.

> **Önemli:** `EOnFrame`, `EOnFixedFrame` gibi callback'ler varsayılan olarak devre dışıdır. Aktif etmek için `SetEventMask()` çağırılmalıdır.

---

## 3. Proje Dosyası: `.gproj`

Her mod, kök dizininde bir `.gproj` (Game Project) dosyasına sahiptir. Bu dosya, Workbench tarafından proje tanımlaması için okunur.

**Gerçek örnek** (`SampleMod_Main/SampleMod.gproj`):
```
GameProject {
 ID "SampleMod"
 GUID "5614E48162D7B84F"
 TITLE "Sample Mod - Main Addon"
 Dependencies {
  "5614E4829EAE4BB1" "5614E482B628B28D" "5614E482B37103E5" ...
 }
 Configurations {
  GameProjectConfig PC {
  }
  GameProjectConfig HEADLESS {
  }
  GameProjectConfig XBOX_ONE {
  }
  GameProjectConfig XBOX_SERIES {
  }
  GameProjectConfig PS4 {
  }
 }
}
```

| Alan | Açıklama |
|---|---|
| `ID` | Projenin okunabilir kimliği (modun klasör adıyla eşleşmeli) |
| `GUID` | Projenin benzersiz kimliği (hexadecimal, 16 karakter) |
| `TITLE` | Workshop ve Workbench'te gösterilen başlık |
| `Dependencies` | Bu modun bağımlı olduğu diğer modların GUID'leri |
| `Configurations` | Hangi platformlar için yapılandırma olduğu |

---

## 4. Dosya Formatları – Tam Liste

| Uzantı | İsim | Araç | Açıklama |
|---|---|---|---|
| `.gproj` | Game Project | Workbench | Mod/proje tanım dosyası |
| `.et` | Entity Template | World Editor / Resource Manager | Prefab; entity hiyerarşisi, bileşenler |
| `.ct` | Component Template | Resource Manager | Tek bileşen şablonu |
| `.conf` | Config | Config Editor | Hiyerarşik yapılandırma (görev, sistem, vb.) |
| `.ent` | World Entity | World Editor | Dünya root dosyası veya SubScene tanımı |
| `.layer` | Layer | World Editor | Entity yerleşim katmanı |
| `.layout` | UI Layout | Layout Editor | UI widget hiyerarşisi |
| `.c` | Enforce Script | Script Editor | Oyun/workbench logic scripti |
| `.xob` | Model | (Import) | 3D model (Enfusion'a özgü) |
| `.edds` | Texture | (Import) | Doku dosyası (Enfusion DDS) |
| `.emat` | Material | Material Editor | Malzeme tanımı |
| `.meta` | Meta | Otomatik | GUID ve platform yapılandırması |

### `.meta` Dosyası – Gerçek Örnek

(`Assets_Showcase_Basic.ent.meta`):
```
MetaFileClass {
 Name "{AA1C7FBA7756E0AC}Worlds/Modding/Assets_Showcase_Base.ent"
 Configurations {
  ENTResourceClass PC {
  }
  ENTResourceClass XBOX_ONE : PC {
  }
  ENTResourceClass PS4 : PC {
  }
  ENTResourceClass HEADLESS : PC {
  }
 }
}
```

Meta dosyaları asset'in GUID'ini (`{AA1C7FBA7756E0AC}`) ve platform yapılandırmalarını saklar. Motor, dosya yolları yerine **GUID ile** asset bağlantılarını takip eder.

---

## 5. Dünya Sistemi: `.ent`, `.layer`, SubScene

### `.ent` – Dünya Root Dosyası

Bir dünya veya senaryo için kök dosyadır. İki türde kullanılır:

**1. Bağımsız Dünya:**  
Doğrudan entity'leri ve katmanları barındırır.

**2. SubScene (Alt Sahne):**  
Mevcut bir dünyaya (örn. Everon haritası) ekleme yapmak için kullanılır. Gerçek örnekten:

```
SubScene {
 Parent "{950F7FA8E622A07B}Worlds/Modding/Assets_Showcase_Base.ent"
}
```

Bu dosya demek ki: "Bu dünya, `Assets_Showcase_Base.ent` üzerine inşa edilmiş."

### `.layer` – Entity Yerleşim Katmanı

`.ent` dosyası, entity'leri doğrudan barındırmaz; bunları `.layer` dosyalarına yönlendirir.

**Gerçek `.layer` dosyası içeriği** (`Assets_Showcase_default.layer`):
```
SCR_PlayerController DefaultPlayerControllerMP1 : "{225E51284CC95CFA}Prefabs/Characters/Core/DefaultPlayerControllerMP.et" {
 coords 1216.248 51.001 1329.951
 SelfRegister 1
}
Vehicle SampleCar_1 : "{1398A24AB596FFE1}Prefabs/Vehicles/Wheeled/SampleCar_01/SampleCar_01.et" {
 coords 1225.29 51.001 1324.695
}
$grp SCR_ChimeraCharacter : "{DCB41B3746FDD1BE}Prefabs/Characters/OPFOR/USSR_Army/Character_USSR_Rifleman.et" {
 PlayerCharacter {
  components {
   CharacterWeaponSlotComponent "{520EA1D2F659CFAB}" {
    WeaponTemplate "{3C399F23FE9B2E7F}Prefabs/Weapons/Rifles/Rifle_AK74_Modded.et"
   }
  }
  coords 1218.183 51.001 1324.075
 }
}
```

**Sözdizimi açıklaması:**
- `EntityClass EntityAdı : "{GUID}Yol/Prefab.et" { ... }` → Prefab'dan instance oluştur
- `coords X Y Z` → Dünya koordinatları (metre)
- `angleY` → Y ekseninde döndürme (derece)
- `components { ... }` → Bu instance'daki bileşen değerlerini override et
- `$grp EntityClass : "..." { { ... } { ... } }` → Aynı prefab'dan birden fazla instance (grup)

**Katman Klasör Yapısı:**

Bir dünya için katmanlar ayrı bir klasörde toplanır:
```
Worlds/Modding/
├── Assets_Showcase_Basic.ent          ← Root dosya
├── Assets_Showcase_Basic.ent.meta
└── Assets_Showcase_Basic_Layers/     ← Katman klasörü
    ├── default.layer                  ← Varsayılan (başlangıç) katmanı
    └── assets.layer                   ← Ek katman
```

> **Kritik Uyarı:** `.layer` dosyaları sıralı olarak işlenir. Bir entity sınıfı tanımı değişirse veya silinirse, o sınıftan sonra gelen tüm entity'ler yüklenemez. Bu nedenle entity sınıfı değişikliklerinde dikkatli olunmalı ve **sürüm kontrolü (Git) kullanılmalıdır**.

### Görev Tanımı: `.conf`

Görev (mission) dosyaları da `.conf` uzantısını kullanır:

```
SCR_MissionHeader {
 World "{AA1C7FBA7756E0AC}Worlds/Modding/Assets_Showcase_Basic.ent"
 m_sName "Sample Mod Showcase - Basic"
 m_sDescription "Asset showcase of various sample mod assets"
 m_sIcon "{134CFE678A529DEB}UI/Textures/MissionThumbnails/Assets_Showcase.edds"
}
```

---

## 6. Prefab Sistemi: `.et` ve `.ct`

### `.et` – Entity Template (Prefab)

Entity Template, Enfusion'un **prefab sistemidir**. Bir `.et` dosyası şunları içerir:
- Entity sınıfı
- Bileşenler ve değerleri
- Alt entity hiyerarşisi (child entities / slot'lar)

**Özellikler:**
- **Inheritance (Miras):** Bir `.et`, başka bir `.et`'i temel alabilir. Sadece farklı değerler override edilir.
- **Propagation:** Prefab değiştirilince tüm instance'lar güncellenir.
- **Slot Sistemi:** Entity'lerin üzerinde tanımlı `EntitySlotInfo` noktalarına başka prefab'lar eklenebilir.

**Gerçek slot örneği** (`.layer` içinden):
```
SlotManagerComponent "{55BCB445DA7D6E35}" {
 Slots {
  EntitySlotInfo Wing_R {
   PivotID "socket_wing_01"
   Prefab "{FA0345CFCE47FCDE}Prefabs/Props/Military/Camps/PortableDesk_01_Sand_Wing.et"
   DisablePhysicsInteraction 0
  }
  EntitySlotInfo Lid_1 {
   PivotID "socket_lid_01"
   Prefab "{433E143A888156C9}Prefabs/Props/Military/Camps/PortableDesk_01_Sand_Lid.et"
  }
 }
}
```

**Workbench'te Prefab Oluşturma:**
1. World Editor'da entity oluştur.
2. Hiyerarşide entity'e sağ tıkla → "Create Prefab From Selection."
3. Kaydet → `.et` dosyası oluşur, yanına `.meta` dosyası otomatik eklenir.

**Prefab'ı Düzenleme:**
1. Resource Browser'dan `.et` dosyasına çift tıkla.
2. "Prefab Edit Mode" açılır.
3. Değişiklikler kaydedilince tüm instance'lar güncellenir.

---

## 7. Konfigürasyon Sistemi: `.conf`

`.conf` dosyaları iki amaçla kullanılır:
1. **Görev tanımı** (`SCR_MissionHeader`)
2. **Sistem/oyun verisi** (silah konfigürasyonu, arsenal ayarları, vb.)

Config Editor ile görsel olarak düzenlenir. Miras sistemi desteklenir: bir `.conf`'u temel alarak sadece değiştirmek istediğin değerleri override edebilirsin.

---

## 8. Script Sistemi: Enforce Script (`.c`)

### Dil Özellikleri

Enforce Script, C# benzeri bir dildir. Temel özellikleri:

- Güçlü tip sistemi
- Sınıf kalıtımı (`class X : Y`)
- `modded` anahtar kelimesi (mevcut sınıfları monkey-patch yapma)
- `override` ile metot ezmesi
- `super` ile üst sınıf metodunu çağırma
- Generic'ler (`array<T>`, `map<K, V>`)
- `ref` ile referans türleri
- `event` işaret anahtar kelimesi (event callback'ler için)
- `thread` ile basit eş zamanlılık

### Script Klasörü → Modül Eşlemesi

| Klasör | Modül | Yüklenme Zamanı | Amaç |
|---|---|---|---|
| `Scripts/Game/` | Game | Oyun sırasında | Gameplay lojiği, entity'ler, bileşenler |
| `Scripts/GameLib/` | GameLib | Oyun sırasında | Kütüphane sınıfları, yardımcılar |
| `Scripts/WorkbenchGame/` | WorkbenchGame | Sadece Workbench'te | Editör pluginleri, araçlar |

> **Kritik:** Dosyalar bu klasörlerin dışına konursa motor tarafından **yüklenmez**.

### Naming Conventions (İsimlendirme Kuralları)

Bohemia Interactive'in resmi kurallarına göre:
- Sınıf adları: `SCR_`, `MyMod_` gibi önek kullan.
- Üye değişkenler: `m_` öneki + tip öneki.
  - `m_b` → bool
  - `m_i` → int
  - `m_f` → float
  - `m_s` → string
  - `m_w` → Widget
  - `m_e` → enum

### `[Attribute]` ile Workbench'e Özellik Açma

Bir sınıfın üye değişkenini Workbench'teki editörde görünür kılmak için `[Attribute]` kullanılır:

```c
[Attribute("0.5", UIWidgets.Slider, "Büyütme çarpanı", "min=0.1; max=5; step=0.1")]
float m_fZoomMultiplier;

[Attribute("", UIWidgets.ResourceAssignArray, "Prefab listesi", "et")]
ref array<ResourceName> m_aPrefabVariants;

[Attribute("0", UIWidgets.CheckBox, "Ölçek rastgeleleştir")]
bool m_bRandomScale;
```

**UIWidgets türleri:** `Slider`, `CheckBox`, `ComboBox`, `EditBox`, `ResourceAssign`, `ResourceAssignArray`, vb.

### `modded` Anahtar Kelimesi

`modded`, mevcut bir oyun sınıfını kaynak koduna dokunmadan değiştirmek için kullanılır. **Gerçek örnek** (SampleMod_ModdedScript'ten):

```c
// "modded" keyword, mevcut sınıfı monkey-patch yapar
modded class SCR_BaseScoringSystemComponent : SCR_BaseGameModeComponent
{
    override void AddSuicide(int playerId, int count = 1)
    {
        // Orijinal lojiği çalıştır
        super.AddSuicide(playerId, count);
        
        // Ek davranış ekle
        AudioSystem.PlaySound("{9BB653FF9E065943}Sounds/Animals/Bos_Taurus/Samples/bawl/bawl_0.wav");
    }
}
```

```c
modded class SCR_ScoringSystemComponent : SCR_BaseScoringSystemComponent
{
    override int CalculateScore(SCR_ScoreInfo info)
    {
        // Orijinal hesaplamayı tamamen değiştir
        int score = info.m_iKills        * m_iKillScoreMultiplier +
                    info.m_iTeamKills    * m_iTeamKillScoreMultiplier +
                    info.m_iDeaths       * 10 +  // override: 10 sabit
                    info.m_iSuicides     * 10 +  // override: 10 sabit
                    info.m_iObjectives   * m_iObjectiveScoreMultiplier;

        if (score < 0)
            return 0;
        return score;
    }
}
```

### Entity ve Component Oluşturma

```c
class MyCustomComponent : ScriptComponent
{
    [Attribute("10", UIWidgets.Slider, "Menzil", "min=1; max=100; step=1")]
    float m_fRange;

    override void OnPostInit(IEntity owner)
    {
        super.OnPostInit(owner);
        // Event mask'ini aktif et (yoksa EOnFrame çağrılmaz)
        SetEventMask(owner, EntityEvent.FRAME);
    }

    override void EOnFrame(IEntity owner, float timeSlice)
    {
        // Her frame'de çağrılır
        vector pos = owner.GetOrigin();
        Print(pos.ToString());
    }
}
```

---

## 9. UI Sistemi: Layout ve Widget'lar

### Layout Dosyası (`.layout`)

`.layout` dosyaları, Layout Editor ile görsel olarak oluşturulur. Bir widget hiyerarşisi tanımlar.

**Widget Türleri (doğrulanmış):**

| Widget | Açıklama |
|---|---|
| `OverlayWidget` | Widget'ları üst üste (z-order ile) yerleştiren kapsayıcı |
| `SizeLayoutWidget` | Alt widget'ların boyutunu ve konumunu yöneten kapsayıcı |
| `ImageWidget` | Statik resim veya ikon gösterir |
| `RichTextWidget` | Formatlanmış metin (renk, stil, boyut desteği) |
| `TextWidget` | Düz metin |
| `ButtonWidget` | Tıklanabilir düğme |
| `FrameWidget` | Çerçeve/panel kapsayıcı |

### Script'ten Widget'a Erişim

Widget'lara script içinden isimleriyle erişilir. Temel pattern:

```c
class MinimapDisplay : SCR_InfoDisplay
{
    // m_w öneki = widget değişkeni
    private ImageWidget m_wMinimapBackground;
    private ImageWidget m_wPlayerIcon;
    private RichTextWidget m_wCoordinateText;

    override event void OnStartDraw(IEntity owner)
    {
        super.OnStartDraw(owner);

        // m_wRoot → layout'un kök widget'ı (SCR_InfoDisplay tarafından sağlanır)
        m_wMinimapBackground = ImageWidget.Cast(m_wRoot.FindAnyWidget("MinimapBackground"));
        m_wPlayerIcon        = ImageWidget.Cast(m_wRoot.FindAnyWidget("PlayerIcon"));
        m_wCoordinateText    = RichTextWidget.Cast(m_wRoot.FindAnyWidget("CoordinateText"));
    }

    override event void OnUpdateDraw(IEntity owner)
    {
        super.OnUpdateDraw(owner);

        if (!m_wPlayerIcon)
            return;

        // Oyuncunun dünya koordinatlarını al
        vector playerPos = owner.GetOrigin();

        // Icon'u minimap üzerinde konumlandır
        // 0.1 = dünya metre → piksel dönüşüm katsayısı (örnek değer; harita boyutuna göre ayarlanmalı)
        WidgetAnimator.SetPos(m_wPlayerIcon, playerPos[0] * 0.1, playerPos[2] * 0.1);

        // Metni güncelle
        if (m_wCoordinateText)
            m_wCoordinateText.SetText(string.Format("X: %1  Z: %2", playerPos[0].ToString(0), playerPos[2].ToString(0)));
    }
}
```

### ButtonWidget Event Handler

```c
private ButtonWidget m_wCloseButton;

override event void OnStartDraw(IEntity owner)
{
    super.OnStartDraw(owner);
    m_wCloseButton = ButtonWidget.Cast(m_wRoot.FindAnyWidget("CloseButton"));
    if (m_wCloseButton)
        m_wCloseButton.m_OnClicked.Insert(OnCloseClicked);
}

protected void OnCloseClicked()
{
    // Kapat lojiği
}
```

> **Not:** HUD (InfoDisplay) widget'ları kullanıcı girişi (tıklama) **almaz** — sadece gösterim içindir. Etkileşim için menü/diyalog context'leri kullanılır.

### HUD'u Oyuncuya Bağlama

HUD'un oyunda görünmesi için player controller prefab'ını override et:

1. Resource Browser'dan `DefaultPlayerControllerMP.et` dosyasını bul.  
   Konum: `{225E51284CC95CFA}Prefabs/Characters/Core/DefaultPlayerControllerMP.et`
2. Sağ tıkla → **Override** → kendi mod klasörüne kaydet.
3. Override edilen prefab'ı aç → SCR_HUDManagerComponent bileşenine gir → kendi `SCR_InfoDisplay` türevini ekle.

---

## 10. Workbench Modülleri ve Plugin Sistemi

### Modüller

| Modül | Sınıf | Açıklama |
|---|---|---|
| World Editor | `WorldEditor` | Harita ve entity düzenleme |
| Resource Manager | `ResourceManager` | Asset yönetimi ve browser |
| Script Editor | `ScriptEditor` | Kod yazma ve debug |
| Layout Editor | Resource Manager içinde | `.layout` dosyaları |
| Particle Editor | `ParticleEditor` | Parçacık sistemi |
| Animation Editor | `AnimationEditor` | Animasyon yönetimi |

### Plugin Türleri

**1. WorldEditorPlugin** – World Editor'da seçili entity'lere erişir.  
**2. ResourceManagerPlugin** – Resource Browser seçimine erişir.  
**3. ScriptEditorPlugin** – Script Editor'da çalışır.  
**4. WorldEditorTool** – Mouse/klavye event'larını yakalar; haritaya interaktif araç ekler.

### Gerçek Plugin Örnekleri (kaynak: BohemiaInteractive/Arma-Reforger-Samples)

**World Editor Plugin:**
```c
[WorkbenchPluginAttribute(name: "Sample World Editor Plugin", 
                          category: "Sample Plugins", 
                          shortcut: "Ctrl+T", 
                          wbModules: {"WorldEditor"})]
class SampleWorldEditorPlugin : WorldEditorPlugin
{
    override void Run()
    {
        // World Editor modülünü al
        WorldEditor worldEditor = Workbench.GetModule(WorldEditor);
        // API'yi al
        WorldEditorAPI api = worldEditor.GetApi();

        // Seçili entity sayısını al ve logla
        int selectedEntitiesCount = api.GetSelectedEntitiesCount();
        Print(selectedEntitiesCount);
    }
}
```

**Resource Manager Plugin (Ayarlar + Butonlar):**
```c
[WorkbenchPluginAttribute(name: "Sample Resource Manager Plugin", 
                          category: "Sample Plugins", 
                          shortcut: "Ctrl+T", 
                          wbModules: {"ResourceManager"}, 
                          awesomeFontCode: 0xf0c5)]
class SampleResourceManagerPlugin : ResourceManagerPlugin
{
    [Attribute("0", UIWidgets.CheckBox, "Çıktıyı panoya kopyala.")]
    bool m_CopyToClipboard;

    [Attribute("0", UIWidgets.CheckBox, "Çıktıyı konsola yaz.")]
    bool m_PrintToConsole;

    [ButtonAttribute("OK")]
    void OkButton() {}

    [ButtonAttribute("Cancel")]
    bool CancelButton() { return false; }

    override void Run()
    {
        ResourceManager resourceManager = Workbench.GetModule(ResourceManager);
        if (!resourceManager) return;

        // Resource Browser'daki seçimi al
        array<ResourceName> selection = new array<ResourceName>;
        resourceManager.GetResourceBrowserSelection(selection.Insert, true);

        if (selection.Count() == 0)
        {
            Print("Resource Browser'da hiçbir şey seçili değil.");
            return;
        }

        if (m_PrintToConsole)
            Print(selection);

        if (m_CopyToClipboard)
        {
            string export;
            foreach (string element : selection)
                export = export + "element: " + element + "\n";
            System.ExportToClipboard(export);
        }
    }

    // Her yeni asset kayıt edildiğinde tetiklenir
    override void OnRegisterResource(string absFileName, BaseContainer metaFile)
    {
        if (m_PrintToConsole)
            Print(absFileName);
    }
}
```

**World Editor Tool (Mouse ile interaktif araç):**
```c
[WorkbenchToolAttribute(name: "Sample World Editor Tool", 
                        description: "Haritaya tıklayarak prefab yerleştir.",
                        wbModules: {"WorldEditor"}, 
                        awesomeFontCode: 0xf074)]
class SampleWorldEditorTool : WorldEditorTool
{
    [Attribute("", UIWidgets.ResourceAssignArray, "Prefab listesi.", "et")]
    protected ref array<ResourceName> m_PrefabVariants;

    [Attribute("0", UIWidgets.CheckBox, "Ölçek rastgeleleştir.")]
    bool m_RandomScale;

    ref array<IEntitySource> m_arrayOfEntities;

    [ButtonAttribute("Delete all")]
    void DeleteAll()
    {
        if (!m_arrayOfEntities) return;
        m_API.BeginEntityAction("Deleting entities");
        m_API.DeleteEntities(m_arrayOfEntities);
        m_API.EndEntityAction();
        m_arrayOfEntities.Clear();
    }

    // Fareye tıklandığında
    override void OnMousePressEvent(float x, float y, WETMouseButtonFlag buttons)
    {
        vector traceStart, traceEnd, traceDir;

        if (!m_PrefabVariants) return;

        if (m_API.TraceWorldPos(x, y, TraceFlags.WORLD, traceStart, traceEnd, traceDir))
        {
            m_API.BeginEntityAction("Entity oluştur");
            // Rastgele bir prefab seç ve dünya koordinatına yerleştir
            IEntitySource entity = m_API.CreateEntity(
                m_PrefabVariants.GetRandomElement(), 
                "", 
                m_API.GetCurrentEntityLayerId(), 
                null, 
                traceEnd, 
                vector.Zero
            );
            m_arrayOfEntities.Insert(entity);

            if (m_RandomScale)
                m_API.SetVariableValue(entity, null, "scale", (Math.RandomFloat(0.5, 2)).ToString());

            m_API.EndEntityAction();
        }
    }

    override void OnActivate()
    {
        m_arrayOfEntities = new array<IEntitySource>;
    }
}
```

---

## 11. Mod Proje Klasör Yapısı (Gerçek Örnek)

Bohemia'nın resmi `SampleMod_Main` projesinden türetilmiş gerçek yapı:

```
<ModAdin>/
├── <ModAdin>.gproj                    ← Proje tanım dosyası (zorunlu)
├── previewImage.jpg                   ← Workshop önizleme görseli
├── thumbnail.png                      ← Küçük resim
│
├── Assets/                            ← Ham asset'ler (import edilmemiş)
├── Common/                            ← Ortak kullanılan dosyalar
│
├── Configs/                           ← Sistem/oyun yapılandırmaları
│   └── Arsenal/
│       └── ArsenalConfig_Sample.conf
│
├── Language/                          ← Yerelleştirme dosyaları
│
├── Missions/                          ← Görev tanım dosyaları
│   ├── SampleMission.conf             ← SCR_MissionHeader
│   └── SampleMission.conf.meta
│
├── Prefabs/                           ← Entity Template'lar
│   ├── Characters/
│   ├── Vehicles/
│   ├── Weapons/
│   └── Systems/
│
├── Scripts/                           ← Enforce Script dosyaları
│   ├── Game/                          ← Oyun lojiği (IEntity, ScriptComponent)
│   │   ├── UI/
│   │   │   └── MinimapDisplay.c
│   │   └── GameMode/
│   │       └── Scoring/
│   └── WorkbenchGame/                 ← Workbench plugin'leri
│       └── Plugins/
│           └── MinimapHelperPlugin.c
│
├── Terrains/                          ← Terrain asset'leri
│
├── UI/                                ← UI dosyaları
│   ├── Textures/                      ← .edds dosyaları
│   └── layouts/                       ← .layout dosyaları (oluşturulacak)
│       └── Minimap.layout
│
└── Worlds/                            ← Dünya dosyaları
    └── MyWorld/
        ├── MyWorld.ent                ← SubScene root
        ├── MyWorld.ent.meta
        └── MyWorld_Layers/
            ├── default.layer          ← Temel entity katmanı
            └── gameplay.layer         ← Gameplay entity'leri
```

---

## 12. HUD/Minimap Geliştirme İş Akışı

### Adım 1: Layout Oluştur

1. Workbench → Resource Manager → `UI/layouts/` klasörüne sağ tıkla → New → Layout.
2. Layout Editor açılır.
3. Widget hiyerarşini oluştur:

```
OverlayWidget (Root)
└── SizeLayoutWidget "MinimapPanel"
    ├── ImageWidget "MinimapBackground"  ← Harita arkaplanı
    ├── ImageWidget "PlayerIcon"         ← Oyuncu ikonu
    └── RichTextWidget "CoordinateText"  ← Koordinat metni
```

4. Her widget'a isim ver (Properties → Name).
5. Konumlandırma için anchor ve pivot'ları ayarla.

### Adım 2: Display Script'i Yaz

`Scripts/Game/UI/MinimapDisplay.c` dosyası oluştur:

```c
class MinimapDisplay : SCR_InfoDisplay
{
    private ImageWidget     m_wMinimapBackground;
    private ImageWidget     m_wPlayerIcon;
    private RichTextWidget  m_wCoordinateText;

    override event void OnStartDraw(IEntity owner)
    {
        super.OnStartDraw(owner);

        if (!m_wRoot)
            return;

        m_wMinimapBackground = ImageWidget.Cast(m_wRoot.FindAnyWidget("MinimapBackground"));
        m_wPlayerIcon        = ImageWidget.Cast(m_wRoot.FindAnyWidget("PlayerIcon"));
        m_wCoordinateText    = RichTextWidget.Cast(m_wRoot.FindAnyWidget("CoordinateText"));
    }

    override event void OnUpdateDraw(IEntity owner)
    {
        super.OnUpdateDraw(owner);

        if (!owner || !m_wPlayerIcon)
            return;

        vector pos = owner.GetOrigin();

        // Dünya koordinatı (metre) → piksel dönüşümü
        // MAP_SCALE: 1 dünya metresi = kaç piksel? (harita ve layout boyutuna göre ayarla)
        // Örneğin 256px layout ve 5120m dünya için: MAP_SCALE = 256 / 5120 = 0.05
        const float MAP_SCALE = 0.05;
        float mapX = pos[0] * MAP_SCALE;
        float mapZ = pos[2] * MAP_SCALE;
        WidgetAnimator.SetPos(m_wPlayerIcon, mapX, mapZ);

        if (m_wCoordinateText)
        {
            m_wCoordinateText.SetText(
                string.Format("X: %1  Z: %2", 
                    pos[0].ToString(0), 
                    pos[2].ToString(0))
            );
        }
    }

    override event void OnStopDraw(IEntity owner)
    {
        super.OnStopDraw(owner);
        // Temizleme lojiği
    }
}
```

### Adım 3: Prefab Override

1. Resource Browser'dan `DefaultPlayerControllerMP.et` bul.
2. Sağ tıkla → Override → `Prefabs/Characters/Core/` klasörüne kaydet.
3. Prefab'ı aç → Entity hiyerarşisinde `SCR_HUDManagerComponent` bileşenini bul.
4. HUD display listesine `MinimapDisplay` ekle, layout dosyasını referans ver.

### Adım 4: Test ve Debug

- Workbench → Play (ya da F5) ile test et.
- Script Editor'daki konsolda `Print()` çıktılarını izle.
- Hata mesajları için Log Console'u kontrol et.

---

## 13. Sık Yapılan Hatalar ve Dikkat Edilecekler

| Hata | Açıklama | Çözüm |
|---|---|---|
| Widget bulunamıyor | `FindAnyWidget` null döndürür | Layout dosyasındaki widget ismiyle script'teki ismin tam eşleştiğini doğrula |
| Script yüklenmiyor | Script doğru klasörde değil | `Scripts/Game/` veya `Scripts/WorkbenchGame/` altında olduğundan emin ol |
| `.layer` bozuluyor | Entity sınıfı yeniden adlandırıldı | Entity sınıf adlarını değiştirmeden önce yedeğini al |
| HUD görünmüyor | Player controller override eksik | `DefaultPlayerControllerMP.et`'i override et ve HUD listesine ekle |
| GUID çakışması | İki asset aynı GUID'e sahip | Her asset'in `.meta` dosyasındaki GUID benzersiz olmalı (Workbench otomatik atar) |
| `EOnFrame` çağrılmıyor | Event mask etkin değil | `SetEventMask(owner, EntityEvent.FRAME)` ekle |
| `modded` çalışmıyor | Dosya sıralaması hatası | `modded` sınıfın orijinal sınıftan **sonra** yüklenmesi gerekir |

---

## 14. Faydalı Kaynaklar

| Kaynak | URL |
|---|---|
| Bohemia Interactive Community Wiki (BIKI) | https://community.bistudio.com/wiki/Arma_Reforger |
| Arma Reforger Dev Hub | https://reforger.armaplatform.com/dev-hub |
| Arma Reforger Samples (GitHub – gerçek kaynak kod) | https://github.com/BohemiaInteractive/Arma-Reforger-Samples |
| Mod Project Setup | https://community.bistudio.com/wiki/Arma_Reforger:Mod_Project_Setup |
| Data Modding Basics | https://community.bistudio.com/wiki/Arma_Reforger:Data_Modding_Basics |
| Prefabs Basics | https://community.bistudio.com/wiki/Arma_Reforger:Prefabs_Basics |
| Layout Creation | https://community.bistudio.com/wiki/Arma_Reforger:Layout_Creation |
| Layout Editor | https://community.bistudio.com/wiki/Arma_Reforger:Resource_Manager:_Layout_Editor |
| Resource Manager | https://community.bistudio.com/wiki/Arma_Reforger:Resource_Manager |
| Workbench Plugin Tutorial | https://community.bistudio.com/wiki/Arma_Reforger:Workbench_Plugin_Tutorial |
| Enforce Script Sözdizimi | https://community.bistudio.com/wiki/Arma_Reforger:Enforce_Script_Syntax |
| Scripting: Keywords | https://community.bistudio.com/wiki/Arma_Reforger:Scripting:_Keywords |
| Scripting: Conventions | https://community.bistudio.com/wiki/Arma_Reforger:Scripting:_Conventions |
| Scripting Modding | https://community.bistudio.com/wiki/Arma_Reforger:Scripting_Modding |
| Entity Lifecycle | https://community.bistudio.com/wiki/Arma_Reforger:Entity_Lifecycle |
| Create an Entity | https://community.bistudio.com/wiki/Arma_Reforger:Create_an_Entity |
| Create a Component | https://community.bistudio.com/wiki/Arma_Reforger:Create_a_Component |
| World Editor | https://community.bistudio.com/wiki/Arma_Reforger:World_Editor |
| File Types | https://community.bistudio.com/wiki/Arma_Reforger:File_Types |
| Modding Boot Camp #4 – UI/HUD | https://reforger.armaplatform.com/news/modding-boot-camp-4-user-interface-and-hud |
| Arma Reforger Script API | https://community.bistudio.com/wikidata/external-data/arma-reforger/ArmaReforgerScriptAPIPublic/ |
| Workbench Video Tutorials | https://www.youtube.com/playlist?list=PLhFQ1b8sqM94_FlgXUWBNz-HUXgab6DuP |
| HUD/UI Addon Rehberi (Steam) | https://steamcommunity.com/sharedfiles/filedetails/?id=2814814735 |
| Arma Reforger Explorer (kaynak referans) | https://arexplorer.zeroy.com/ |
