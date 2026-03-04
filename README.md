# Minimaphud

## Vanilla Veri Kurulumu

Arma Reforger Tools'taki gerçek haritaları, prefabları, layoutları ve scriptleri
`vanilla-data/` klasörüne kopyalamak için kurulum scriptini çalıştırın.

**Windows:**
```powershell
.\scripts\setup-vanilla-data.ps1
```

**Linux / macOS (örn. Proton ile):**
```bash
chmod +x scripts/setup-vanilla-data.sh
./scripts/setup-vanilla-data.sh
```

Kurulum tamamlandıktan sonra `vanilla-data\ArmaReforger\ArmaReforger.gproj`
dosyasını Workbench'te **File → Open Project** ile açın.

Ayrıntılar için: [`vanilla-data/README.md`](vanilla-data/README.md)

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

