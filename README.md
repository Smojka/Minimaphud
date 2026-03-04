# Minimaphud

Arma Reforger için geliştirilmekte olan bir Minimap HUD modu.

## Proje Yapısı

```
Minimaphud/
├── addon.gproj                  ← Workbench proje dosyası
├── Scripts/
│   └── Game/
│       └── UI/
│           └── MinimapHUD.c    ← Ana HUD script sınıfı (SCR_InfoDisplay)
├── UI/
│   └── Layouts/
│       └── MinimapHUD.layout   ← HUD widget düzeni
├── Prefabs/
│   └── HUD/
│       └── MinimapHUD.et       ← HUD entity prefabı
├── vanilla-data/               ← Vanilla oyun verileri (yerel, Git'e gönderilmez)
│   └── README.md               ← Nasıl doldurulacağını açıklar
└── scripts/
    ├── setup-vanilla-data.ps1  ← Windows: vanilla dosyaları kopyalar
    └── setup-vanilla-data.sh   ← Linux/macOS: vanilla dosyaları kopyalar
```

## Geliştirme Ortamı Kurulumu

### 1. Vanilla Veriyi Kopyala

Bu adım, Arma Reforger araçlarındaki gerçek haritaları, prefabları,
layoutları ve scriptleri `vanilla-data/` klasörüne kopyalar; böylece
hem Workbench hem de AI ajanı gerçek dosyalar üzerinden çalışır.

**Windows:**
```powershell
.\scripts\setup-vanilla-data.ps1
```

**Linux / macOS (örn. Proton ile):**
```bash
chmod +x scripts/setup-vanilla-data.sh
./scripts/setup-vanilla-data.sh
```

### 2. Workbench'e Projeyi Ekle

1. Arma Reforger Tools → Workbench'i açın
2. **File → Open Project** → `vanilla-data\ArmaReforger\ArmaReforger.gproj`
3. **File → Open Project** → `addon.gproj` (bu mod)
4. Workbench artık vanilla tüm prefab/script/layout dosyalarını ve
   bu modun dosyalarını birlikte görecektir

### 3. Mod Geliştir

- `Scripts/Game/UI/MinimapHUD.c` — HUD mantığını düzenle
- `UI/Layouts/MinimapHUD.layout` — Workbench Layout Editor'da düzenle
- `Prefabs/HUD/MinimapHUD.et` — Prefab Editor'da düzenle

## Vanilla Veri Hakkında

`vanilla-data/` klasörü Bohemia Interactive'in telif hakkı kapsamındadır ve
GitHub'a **yüklenmez** — her geliştirici kendi makinesinde kurulum scriptini çalıştırır.

Kopyalanan dosya türleri (metin tabanlı, AI için kritik):

| Uzantı | Açıklama |
|--------|----------|
| `.c` | Enforce Script kaynak kodları |
| `.et` | Entity/Prefab şablonları |
| `.layout` | UI widget düzeni tanımları |
| `.conf` | Config dosyaları |
| `.ent` | World/SubScene kök dosyaları |
| `.layer` | Entity yerleştirme katmanları |
| `.gproj` | Proje tanım dosyaları |
| `.meta` | GUID meta dosyaları |
