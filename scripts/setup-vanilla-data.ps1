# =============================================================================
#  setup-vanilla-data.ps1
#  Arma Reforger Tools — vanilla metin dosyalarını projeye kopyalar.
#
#  Kopyalanan dosya türleri (metin tabanlı, AI referansı için gerekli):
#    .c        – Enforce Script kaynak kodları
#    .et       – Entity/Prefab şablonları
#    .layout   – UI widget düzeni tanımları
#    .conf     – Config dosyaları (görev, tur ayarları vb.)
#    .ent      – World/SubScene kök dosyaları
#    .layer    – Entity yerleştirme katmanları
#    .gproj    – Proje tanım dosyası
#    .meta     – GUID meta dosyaları
#
#  Kopyalanmayan dosya türleri (binary, AI için gereksiz, çok büyük):
#    .xob, .edds, .emat, .pak, .dll, .exe, .p3d …
#
#  Kullanım:
#    PowerShell'i yönetici olarak açın ve çalıştırın:
#    .\scripts\setup-vanilla-data.ps1
#
#  İsteğe bağlı parametreler:
#    -GamePath     "D:\Steam\steamapps\common\Arma Reforger"
#    -OutputPath   ".\vanilla-data"
# =============================================================================

param(
    [string]$GamePath   = "",
    [string]$OutputPath = ""
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

# ---------------------------------------------------------------------------
# Yardımcı fonksiyonlar
# ---------------------------------------------------------------------------
function Write-Step($msg) { Write-Host "`n==> $msg" -ForegroundColor Cyan }
function Write-Ok($msg)   { Write-Host "    [OK] $msg" -ForegroundColor Green }
function Write-Warn($msg) { Write-Host "    [!!] $msg" -ForegroundColor Yellow }

# ---------------------------------------------------------------------------
# Çıktı dizinini belirle
# ---------------------------------------------------------------------------
if (-not $OutputPath) {
    $OutputPath = Join-Path $PSScriptRoot "..\vanilla-data"
}
$OutputPath = [System.IO.Path]::GetFullPath($OutputPath)

# ---------------------------------------------------------------------------
# Arma Reforger oyun yolunu bul
# ---------------------------------------------------------------------------
Write-Step "Arma Reforger kurulum dizini aranıyor..."

$candidates = @(
    $GamePath,
    "$Env:ProgramFiles\Steam\steamapps\common\Arma Reforger",
    "${Env:ProgramFiles(x86)}\Steam\steamapps\common\Arma Reforger",
    "D:\Steam\steamapps\common\Arma Reforger",
    "E:\Steam\steamapps\common\Arma Reforger",
    "C:\Games\Steam\steamapps\common\Arma Reforger"
)

# Steam libraryfolders.vdf'den de okumayı dene
$vdfPaths = @(
    "${Env:ProgramFiles(x86)}\Steam\steamapps\libraryfolders.vdf",
    "$Env:ProgramFiles\Steam\steamapps\libraryfolders.vdf"
)
foreach ($vdf in $vdfPaths) {
    if (Test-Path $vdf) {
        $lines = Get-Content $vdf | Select-String '"path"'
        foreach ($line in $lines) {
            if ($line -match '"path"\s+"(.+)"') {
                $libPath = $Matches[1].Replace("\\\\", "\")
                $candidates += "$libPath\steamapps\common\Arma Reforger"
            }
        }
    }
}

$reforgerRoot = $null
foreach ($c in $candidates) {
    if ($c -and (Test-Path "$c\addons\data\ArmaReforger.gproj")) {
        $reforgerRoot = $c
        break
    }
}

if (-not $reforgerRoot) {
    Write-Warn "Arma Reforger otomatik bulunamadı."
    Write-Warn "Lütfen oyunun kurulu olduğu dizini girin (örn: D:\Steam\steamapps\common\Arma Reforger):"
    $reforgerRoot = Read-Host "Oyun dizini"
    if (-not (Test-Path "$reforgerRoot\addons\data\ArmaReforger.gproj")) {
        Write-Host "`n[HATA] Geçerli bir Arma Reforger dizini bulunamadı. Scripti durduruluyor." -ForegroundColor Red
        exit 1
    }
}

$vanillaDataSrc = Join-Path $reforgerRoot "addons\data"
Write-Ok "Vanilla veri kaynağı: $vanillaDataSrc"
Write-Ok "Hedef dizin       : $OutputPath"

# ---------------------------------------------------------------------------
# Kopyalanacak metin tabanlı uzantılar
# ---------------------------------------------------------------------------
$textExtensions = @(
    "*.c",
    "*.et",
    "*.layout",
    "*.conf",
    "*.ent",
    "*.layer",
    "*.gproj",
    "*.meta"
)

# ---------------------------------------------------------------------------
# Kopyalama işlemi
# ---------------------------------------------------------------------------
Write-Step "Vanilla metin dosyaları kopyalanıyor..."

$totalCopied = 0
$totalSkipped = 0

foreach ($pattern in $textExtensions) {
    $ext = $pattern.TrimStart("*")
    $destSubDir = Join-Path $OutputPath "ArmaReforger"

    $files = Get-ChildItem -Path $vanillaDataSrc -Filter $pattern -Recurse -ErrorAction SilentlyContinue
    foreach ($file in $files) {
        # Kaynak kökünden itibaren göreli yol oluştur
        $relativePath = $file.FullName.Substring($vanillaDataSrc.Length).TrimStart('\', '/')
        $destFile = Join-Path $destSubDir $relativePath

        $destDir = Split-Path $destFile -Parent
        if (-not (Test-Path $destDir)) {
            New-Item -ItemType Directory -Path $destDir -Force | Out-Null
        }

        # Zaten varsa ve özdeşse kopyalama
        if ((Test-Path $destFile) -and
            ((Get-Item $destFile).Length -eq $file.Length) -and
            ((Get-Item $destFile).LastWriteTime -eq $file.LastWriteTime)) {
            $totalSkipped++
            continue
        }

        Copy-Item -Path $file.FullName -Destination $destFile -Force
        $totalCopied++
    }
}

Write-Ok "Kopyalanan : $totalCopied dosya"
Write-Ok "Atlanan    : $totalSkipped dosya (değişmemiş)"

# ---------------------------------------------------------------------------
# Proje GUID kataloğu oluştur
# ---------------------------------------------------------------------------
Write-Step "GUID kataloğu oluşturuluyor..."

$catalogPath = Join-Path $OutputPath "guid-catalog.txt"
$gprojFiles  = Get-ChildItem -Path (Join-Path $OutputPath "ArmaReforger") -Filter "*.gproj" -Recurse -ErrorAction SilentlyContinue
$metaFiles   = Get-ChildItem -Path (Join-Path $OutputPath "ArmaReforger") -Filter "*.meta"  -Recurse -ErrorAction SilentlyContinue

$catalogLines = @()
$catalogLines += "# Arma Reforger Vanilla GUID Kataloğu"
$catalogLines += "# Otomatik oluşturuldu: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')"
$catalogLines += "# Format: GUID | Dosya yolu"
$catalogLines += ""

foreach ($meta in $metaFiles) {
    $content = Get-Content $meta.FullName -Raw -ErrorAction SilentlyContinue
    if ($content -match 'guid\s+"([0-9A-F]{16})"') {
        $guid = $Matches[1]
        $rel  = $meta.FullName.Substring((Join-Path $OutputPath "ArmaReforger").Length).TrimStart('\','/').Replace(".meta","")
        $catalogLines += "$guid | $rel"
    }
}

$catalogLines | Set-Content -Path $catalogPath -Encoding UTF8
Write-Ok "GUID kataloğu: $catalogPath ($($catalogLines.Count - 4) giriş)"

# ---------------------------------------------------------------------------
# Özet
# ---------------------------------------------------------------------------
Write-Host ""
$shortPath = if ($OutputPath.Length -gt 48) { "..." + $OutputPath.Substring($OutputPath.Length - 45) } else { $OutputPath }
$shortCount = "$totalCopied kopyalandı"
Write-Host "╔══════════════════════════════════════════════════════════════╗" -ForegroundColor Green
Write-Host "║  Vanilla veri kurulumu tamamlandı!                          ║" -ForegroundColor Green
Write-Host "║                                                              ║" -ForegroundColor Green
Write-Host ("║  Dizin : " + $shortPath.PadRight(50) + " ║") -ForegroundColor Green
Write-Host ("║  Dosya : " + $shortCount.PadRight(50) + " ║") -ForegroundColor Green
Write-Host "╚══════════════════════════════════════════════════════════════╝" -ForegroundColor Green
Write-Host ""
Write-Host "Sonraki adım: Workbench'te File > Open Project ile" -ForegroundColor White
Write-Host "vanilla-data\ArmaReforger\ArmaReforger.gproj dosyasını ekleyin." -ForegroundColor White
