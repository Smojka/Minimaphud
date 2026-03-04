# vanilla-data/

Bu klasör **Arma Reforger Tools** (Steam) tarafından sağlanan vanilla oyun verilerini içerir.

## İçerik

Kurulum scriptini çalıştırdıktan sonra bu klasör şu yapıyı alır:

```
vanilla-data/
├── ArmaReforger/           ← Vanilla oyun verileri (script çalıştırınca dolacak)
│   ├── Scripts/
│   │   └── Game/           ← Tüm vanilla .c scriptleri (SCR_InfoDisplay, vb.)
│   ├── Prefabs/            ← Tüm vanilla .et prefab dosyaları
│   ├── UI/
│   │   └── Layouts/        ← Tüm vanilla .layout HUD/UI dosyaları
│   ├── Configs/            ← Vanilla config dosyaları (.conf)
│   ├── Worlds/             ← Dünya ve SubScene dosyaları (.ent, .layer)
│   └── ArmaReforger.gproj  ← Vanilla proje dosyası
└── guid-catalog.txt        ← Tüm vanilla varlıkların GUID kataloğu
```

## Kurulum

### Windows (PowerShell)

```powershell
cd <proje-kök-dizini>
.\scripts\setup-vanilla-data.ps1
```

### Linux / macOS (Bash)

```bash
chmod +x scripts/setup-vanilla-data.sh
./scripts/setup-vanilla-data.sh
```

### Manuel kurulum

Arma Reforger Tools zaten kuruluysa, aşağıdaki kaynaktan dosyaları kopyalayın:

```
C:\Program Files (x86)\Steam\steamapps\common\Arma Reforger\addons\data\
                                                                  └──────── buradan
                                                                  ↓
vanilla-data\ArmaReforger\
```

## Neden bu klasör boş?

Bu klasördeki dosyalar Bohemia Interactive'in telif hakkı kapsamındadır ve
GitHub'a **yüklenmez** (`.gitignore` tarafından hariç tutulur).

Her geliştirici kendi makinesinde scripti çalıştırarak bu klasörü doldurur.

## Workbench'e ekleme

1. Workbench'i açın
2. **File → Open Project** (veya **Add Existing**)
3. `vanilla-data\ArmaReforger\ArmaReforger.gproj` dosyasını seçin
4. Artık AI ajanı ve Workbench vanilla tüm prefab/script/layout dosyalarını görür

## Vanilla veri konumu (referans)

| Kaynak | Yol |
|--------|-----|
| Vanilla data root | `<Steam>\steamapps\common\Arma Reforger\addons\data\` |
| Vanilla proje | `...\addons\data\ArmaReforger.gproj` |
| Vanilla scriptler | `...\addons\data\Scripts\` |
| Vanilla prefablar | `...\addons\data\Prefabs\` |
| Vanilla layoutlar | `...\addons\data\UI\Layouts\` |
| Vanilla config'ler | `...\addons\data\Configs\` |
| Vanilla dünyalar | `...\addons\data\Worlds\` |
