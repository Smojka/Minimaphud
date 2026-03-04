#!/usr/bin/env bash
# =============================================================================
#  setup-vanilla-data.sh
#  Linux / macOS — Arma Reforger vanilla metin dosyalarını projeye kopyalar.
#
#  Kopyalanan: .c  .et  .layout  .conf  .ent  .layer  .gproj  .meta
#  Atlanır   : .xob  .edds  .emat  ve diğer binary dosyalar
#
#  Kullanım:
#    chmod +x scripts/setup-vanilla-data.sh
#    ./scripts/setup-vanilla-data.sh
#
#  İsteğe bağlı:
#    GAME_PATH=/custom/path ./scripts/setup-vanilla-data.sh
# =============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
OUTPUT_PATH="${PROJECT_ROOT}/vanilla-data"

GREEN='\033[0;32m'
CYAN='\033[0;36m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

step()  { echo -e "\n${CYAN}==> $*${NC}"; }
ok()    { echo -e "    ${GREEN}[OK] $*${NC}"; }
warn()  { echo -e "    ${YELLOW}[!!] $*${NC}"; }
error() { echo -e "${RED}[HATA] $*${NC}"; exit 1; }

# ---------------------------------------------------------------------------
# Oyun dizinini bul
# ---------------------------------------------------------------------------
step "Arma Reforger kurulum dizini aranıyor..."

CANDIDATES=(
    "${GAME_PATH:-}"
    "$HOME/.steam/steam/steamapps/common/Arma Reforger"
    "$HOME/.local/share/Steam/steamapps/common/Arma Reforger"
    "/mnt/c/Program Files (x86)/Steam/steamapps/common/Arma Reforger"
    "/mnt/d/Steam/steamapps/common/Arma Reforger"
    "/Applications/Steam/steamapps/common/Arma Reforger"
)

REFORGER_ROOT=""
for c in "${CANDIDATES[@]}"; do
    if [[ -n "$c" && -f "$c/addons/data/ArmaReforger.gproj" ]]; then
        REFORGER_ROOT="$c"
        break
    fi
done

if [[ -z "$REFORGER_ROOT" ]]; then
    warn "Arma Reforger otomatik bulunamadı."
    read -rp "Oyun dizinini girin: " REFORGER_ROOT
    if [[ ! -f "$REFORGER_ROOT/addons/data/ArmaReforger.gproj" ]]; then
        error "Geçerli Arma Reforger dizini bulunamadı."
    fi
fi

VANILLA_SRC="$REFORGER_ROOT/addons/data"
VANILLA_DEST="$OUTPUT_PATH/ArmaReforger"

ok "Kaynak: $VANILLA_SRC"
ok "Hedef : $VANILLA_DEST"

# ---------------------------------------------------------------------------
# Metin tabanlı dosyaları kopyala
# ---------------------------------------------------------------------------
step "Vanilla metin dosyaları kopyalanıyor..."

TEXT_EXTENSIONS=("c" "et" "layout" "conf" "ent" "layer" "gproj" "meta")

TOTAL_COPIED=0
TOTAL_SKIPPED=0

for ext in "${TEXT_EXTENSIONS[@]}"; do
    while IFS= read -r -d '' file; do
        rel="${file#$VANILLA_SRC/}"
        dest="$VANILLA_DEST/$rel"
        dest_dir="$(dirname "$dest")"
        mkdir -p "$dest_dir"

        if [[ -f "$dest" ]] && \
           [[ "$(stat -c%s "$file" 2>/dev/null || stat -f%z "$file")" == \
              "$(stat -c%s "$dest" 2>/dev/null || stat -f%z "$dest")" ]]; then
            ((TOTAL_SKIPPED++)) || true
            continue
        fi

        cp "$file" "$dest"
        ((TOTAL_COPIED++)) || true
    done < <(find "$VANILLA_SRC" -type f -name "*.$ext" -print0)
done

ok "Kopyalanan : $TOTAL_COPIED dosya"
ok "Atlanan    : $TOTAL_SKIPPED dosya (değişmemiş)"

# ---------------------------------------------------------------------------
# GUID kataloğu
# ---------------------------------------------------------------------------
step "GUID kataloğu oluşturuluyor..."

CATALOG="$OUTPUT_PATH/guid-catalog.txt"
{
    echo "# Arma Reforger Vanilla GUID Kataloğu"
    echo "# Otomatik oluşturuldu: $(date '+%Y-%m-%d %H:%M:%S')"
    echo "# Format: GUID | Dosya yolu"
    echo ""
    find "$VANILLA_DEST" -name "*.meta" -print0 | while IFS= read -r -d '' meta; do
        guid=$(grep -oP 'guid\s+"\K[0-9A-F]{16}' "$meta" 2>/dev/null || true)
        if [[ -n "$guid" ]]; then
            rel="${meta#$VANILLA_DEST/}"
            echo "$guid | ${rel%.meta}"
        fi
    done
} > "$CATALOG"

ok "GUID kataloğu: $CATALOG"

# ---------------------------------------------------------------------------
# Özet
# ---------------------------------------------------------------------------
echo ""
printf "${GREEN}╔══════════════════════════════════════════════════════════════╗\n${NC}"
printf "${GREEN}║  Vanilla veri kurulumu tamamlandı!                          ║\n${NC}"
printf "${GREEN}║  %-58s ║\n${NC}" "Kopyalanan: $TOTAL_COPIED dosya"
printf "${GREEN}╚══════════════════════════════════════════════════════════════╝\n${NC}"
echo ""
echo "Sonraki adım: Workbench > File > Open Project:"
echo "  $VANILLA_DEST/ArmaReforger.gproj"
