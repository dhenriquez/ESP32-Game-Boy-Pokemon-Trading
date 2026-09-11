#pragma once

#include <pgmspace.h>

static const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 Game Boy Pokémon Trader</title>
  <link rel="preconnect" href="https://fonts.googleapis.com">
  <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
  <link href="https://fonts.googleapis.com/css2?family=Outfit:wght@400;500;600;700;800&family=Press+Start+2P&display=swap" rel="stylesheet">
  <style>
    :root {
      --bg: #0d1117;
      --card-bg: rgba(26, 32, 44, 0.85);
      --card-border: rgba(255, 255, 255, 0.08);
      --card-border-glow: rgba(255, 203, 5, 0.3);
      --poke-yellow: #ffcb05;
      --poke-blue: #3c5aa6;
      --poke-dark-blue: #2a447f;
      --poke-red: #ee1515;
      --text: #f7fafc;
      --text-muted: #94a3b8;
      --status-red: #ef4444;
      --status-green: #22c55e;
      --status-yellow: #eab308;
      --status-blue: #3b82f6;
    }

    * { box-sizing: border-box; margin: 0; padding: 0; font-family: 'Outfit', -apple-system, sans-serif; }
    
    body {
      background: radial-gradient(circle at top, #1a202c 0%, #0d1117 100%);
      color: var(--text);
      min-height: 100vh;
      padding: 14px;
      display: flex;
      flex-direction: column;
      align-items: center;
    }

    .container {
      width: 100%;
      max-width: 640px;
    }

    header {
      text-align: center;
      margin-bottom: 16px;
    }
        .brand-logo {
      max-width: 250px;
      width: 100%;
      height: auto;
      display: block;
      margin: 0 auto 8px auto;
      filter: drop-shadow(0 4px 10px rgba(0, 0, 0, 0.5));
    }
    .brand-title {
      font-family: 'Press Start 2P', cursive, monospace;
      font-size: 1.1rem;
      color: var(--poke-yellow);
      text-shadow: 2px 3px 0px var(--poke-blue), 3px 4px 0px #000;
      letter-spacing: 1px;
      margin-bottom: 6px;
    }
    .brand-subtitle {
      font-size: 0.82rem;
      color: var(--text-muted);
      font-weight: 500;
    }

    .status-card {
      background: var(--card-bg);
      backdrop-filter: blur(12px);
      border: 1px solid var(--card-border);
      border-radius: 16px;
      padding: 14px 18px;
      margin-bottom: 14px;
      display: flex;
      align-items: center;
      justify-content: space-between;
      box-shadow: 0 10px 25px rgba(0, 0, 0, 0.3);
      transition: border-color 0.3s;
    }
    .status-card.connected {
      border-color: rgba(34, 197, 94, 0.4);
      box-shadow: 0 10px 25px rgba(34, 197, 94, 0.15);
    }
    .status-left {
      display: flex;
      align-items: center;
      gap: 12px;
    }
    .status-pulse {
      width: 14px;
      height: 14px;
      border-radius: 50%;
      background: var(--status-red);
      box-shadow: 0 0 10px var(--status-red);
      transition: all 0.3s;
    }
    .status-card.connected .status-pulse {
      background: var(--status-green);
      box-shadow: 0 0 12px var(--status-green);
      animation: pulse 1.6s infinite;
    }
    @keyframes pulse {
      0% { opacity: 0.5; transform: scale(0.95); }
      50% { opacity: 1; transform: scale(1.1); }
      100% { opacity: 0.5; transform: scale(0.95); }
    }
    .status-title {
      font-weight: 700;
      font-size: 0.95rem;
    }
    .status-desc {
      font-size: 0.75rem;
      color: var(--text-muted);
    }
    .gen-badge {
      background: linear-gradient(135deg, var(--poke-blue), var(--poke-dark-blue));
      color: #fff;
      font-size: 0.75rem;
      font-weight: 800;
      padding: 6px 12px;
      border-radius: 20px;
      border: 1px solid rgba(255, 255, 255, 0.15);
      letter-spacing: 0.5px;
    }

    .tabs {
      display: grid;
      grid-template-columns: repeat(4, 1fr);
      gap: 6px;
      margin-bottom: 14px;
    }
    .tab-btn {
      background: var(--card-bg);
      border: 1px solid var(--card-border);
      color: var(--text-muted);
      padding: 10px 4px;
      border-radius: 12px;
      cursor: pointer;
      font-size: 0.8rem;
      font-weight: 600;
      text-align: center;
      transition: all 0.2s;
    }
    .tab-btn.active {
      background: rgba(45, 55, 72, 0.9);
      color: var(--poke-yellow);
      border-color: var(--poke-yellow);
      box-shadow: 0 4px 14px rgba(255, 203, 5, 0.15);
    }

    .tab-pane { display: none; }
    .tab-pane.active { display: block; animation: fadeIn 0.25s ease-in-out; }
    @keyframes fadeIn { from { opacity: 0; transform: translateY(4px); } to { opacity: 1; transform: translateY(0); } }

    .card {
      background: var(--card-bg);
      border: 1px solid var(--card-border);
      border-radius: 16px;
      padding: 18px;
      margin-bottom: 14px;
      box-shadow: 0 8px 20px rgba(0, 0, 0, 0.25);
    }
    .card-title {
      font-size: 0.95rem;
      font-weight: 700;
      color: var(--poke-yellow);
      margin-bottom: 14px;
      display: flex;
      align-items: center;
      gap: 8px;
    }

    .presets-grid {
      display: flex;
      flex-wrap: wrap;
      gap: 7px;
    }
    .preset-pill {
      background: rgba(45, 55, 72, 0.6);
      border: 1px solid rgba(255, 255, 255, 0.1);
      color: #e2e8f0;
      font-size: 0.75rem;
      font-weight: 500;
      padding: 7px 11px;
      border-radius: 20px;
      cursor: pointer;
      transition: all 0.2s;
    }
    .preset-pill:hover {
      background: var(--poke-blue);
      border-color: var(--poke-yellow);
      color: #fff;
    }

    .pkmn-preview-box {
      display: flex;
      align-items: center;
      background: rgba(15, 23, 42, 0.6);
      border: 1px solid rgba(255, 255, 255, 0.05);
      border-radius: 12px;
      padding: 12px;
      margin-bottom: 16px;
      gap: 16px;
    }
    .pkmn-sprite {
      width: 76px;
      height: 76px;
      background: radial-gradient(circle, rgba(255,203,5,0.15) 0%, transparent 70%);
      border-radius: 10px;
      display: flex;
      align-items: center;
      justify-content: center;
      transition: all 0.3s;
    }
    .pkmn-sprite.shiny {
      background: radial-gradient(circle, rgba(255, 203, 5, 0.35) 0%, rgba(245, 158, 11, 0.1) 70%);
      box-shadow: 0 0 15px rgba(255, 203, 5, 0.4);
      border: 1px solid rgba(255, 203, 5, 0.5);
    }
    .pkmn-sprite img {
      width: 68px;
      height: 68px;
      image-rendering: pixelated;
      filter: drop-shadow(0 4px 6px rgba(0,0,0,0.5));
    }
    .pkmn-summary-name {
      font-size: 1.1rem;
      font-weight: 800;
      color: #fff;
      display: flex;
      align-items: center;
      gap: 8px;
    }
    .pkmn-summary-dex {
      font-size: 0.75rem;
      color: var(--poke-yellow);
      font-weight: 700;
      background: rgba(255, 203, 5, 0.1);
      padding: 2px 6px;
      border-radius: 6px;
    }
    .pkmn-summary-details {
      font-size: 0.8rem;
      color: var(--text-muted);
      margin-top: 4px;
    }

    .grid-2 {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 12px;
    }
    .form-group {
      margin-bottom: 14px;
    }
    label {
      display: block;
      font-size: 0.72rem;
      color: var(--text-muted);
      font-weight: 600;
      margin-bottom: 5px;
      text-transform: uppercase;
      letter-spacing: 0.5px;
    }
    input, select {
      width: 100%;
      background: rgba(15, 23, 42, 0.8);
      border: 1px solid rgba(255, 255, 255, 0.1);
      color: var(--text);
      border-radius: 8px;
      padding: 9px 12px;
      font-size: 0.88rem;
      font-weight: 500;
    }
    input:focus, select:focus {
      outline: none;
      border-color: var(--poke-yellow);
      box-shadow: 0 0 0 2px rgba(255, 203, 5, 0.15);
    }

    .btn {
      width: 100%;
      padding: 13px;
      border-radius: 10px;
      border: none;
      font-size: 0.95rem;
      font-weight: 700;
      cursor: pointer;
      text-align: center;
      transition: all 0.2s;
    }
    .btn-primary {
      background: linear-gradient(135deg, #e53e3e, #c53030);
      color: white;
      box-shadow: 0 4px 15px rgba(229, 62, 62, 0.3);
    }
    .btn-primary:hover {
      background: linear-gradient(135deg, #f56565, #e53e3e);
    }
    .btn-success {
      background: linear-gradient(135deg, #38a169, #2f855a);
      color: white;
      box-shadow: 0 4px 15px rgba(56, 161, 105, 0.3);
    }
    .btn-success:hover {
      background: linear-gradient(135deg, #48bb78, #38a169);
    }

    .console-box {
      background: #090d13;
      color: #38ef7d;
      font-family: 'Courier New', Courier, monospace;
      font-size: 0.78rem;
      padding: 12px;
      border-radius: 10px;
      height: 160px;
      overflow-y: auto;
      border: 1px solid rgba(255, 255, 255, 0.08);
      line-height: 1.5;
    }

    .switch-label {
      display: flex;
      align-items: center;
      justify-content: space-between;
      cursor: pointer;
      user-select: none;
      background: rgba(15, 23, 42, 0.5);
      border: 1px solid rgba(255, 255, 255, 0.05);
      padding: 10px 14px;
      border-radius: 8px;
    }

    .toast {
      position: fixed;
      bottom: 24px;
      left: 50%;
      transform: translateX(-50%) translateY(100px);
      background: #1e293b;
      border: 1px solid var(--poke-yellow);
      color: #fff;
      padding: 12px 24px;
      border-radius: 30px;
      font-size: 0.85rem;
      font-weight: 600;
      box-shadow: 0 10px 25px rgba(0, 0, 0, 0.5);
      transition: all 0.3s cubic-bezier(0.68, -0.55, 0.27, 1.55);
      z-index: 100;
      opacity: 0;
      pointer-events: none;
    }
    .toast.show {
      transform: translateX(-50%) translateY(0);
      opacity: 1;
    }
  </style>
</head>
<body>
  <div class="container">
    <header>
      <img src="/logo.png" alt="GB-PokeTrader" class="brand-logo">
      <div class="brand-subtitle">by @dhenriquez</div>
    </header>

    <div class="status-card" id="statusCard">
      <div class="status-left">
        <div class="status-pulse" id="statusDot"></div>
        <div>
          <div class="status-title" id="statusTitle">En espera de Game Boy...</div>
          <div class="status-desc" id="statusDesc">Conecta el cable Link al Game Boy y entra al Club del Cable</div>
        </div>
      </div>
      <div class="gen-badge" id="genBadge">GEN I</div>
    </div>

    <div class="tabs">
      <button class="tab-btn active" onclick="switchTab('tab-inject', this)">⚡ Enviar</button>
      <button class="tab-btn" onclick="switchTab('tab-receive', this)">🔄 Recibido</button>
      <button class="tab-btn" onclick="switchTab('tab-monitor', this)">📡 Monitor</button>
      <button class="tab-btn" onclick="switchTab('tab-config', this)">⚙️ Ajustes</button>
    </div>

    <!-- TAB 1: ENVIAR -->
    <div id="tab-inject" class="tab-pane active">
      <div class="card">
        <div class="card-title">🚀 Presets de Intercambio Instantáneos</div>
        <div class="presets-grid">
          <span class="preset-pill" onclick="applyPreset('gengar')">👻 Haunter ➔ Gengar</span>
          <span class="preset-pill" onclick="applyPreset('machamp')">💪 Machoke ➔ Machamp</span>
          <span class="preset-pill" onclick="applyPreset('alakazam')">🥄 Kadabra ➔ Alakazam</span>
          <span class="preset-pill" onclick="applyPreset('golem')">🪨 Graveler ➔ Golem</span>
          <span class="preset-pill" onclick="applyPreset('steelix')">🔩 Onix ➔ Steelix</span>
          <span class="preset-pill" onclick="applyPreset('scizor')">✂️ Scyther ➔ Scizor</span>
          <span class="preset-pill" onclick="applyPreset('kingdra')">🐉 Seadra ➔ Kingdra</span>
          <span class="preset-pill" onclick="applyPreset('mew')">✨ Mew de Evento</span>
          <span class="preset-pill" onclick="applyPreset('celebi')">🍃 Celebi de Evento</span>
        </div>
      </div>

      <div class="card">
        <div class="card-title">📝 Personalizar Pokémon a Enviar</div>
        <div class="pkmn-preview-box">
          <div class="pkmn-sprite">
            <img id="spriteImg" src="/sprites/094.png" alt="Pokemon Sprite" onerror="this.src='/sprites/000.png'">
          </div>
          <div>
            <div class="pkmn-summary-name">
              <span id="previewName">GENGAR</span>
              <span class="pkmn-summary-dex" id="previewDex">#094</span>
            </div>
            <div class="pkmn-summary-details" id="previewDetails">Nivel 50 • Fantasma / Veneno • Max IV/EV</div>
          </div>
        </div>

        <form id="pkmnForm" onsubmit="savePokemon(event)">
          <div class="grid-2">
            <div class="form-group">
              <label>Especie</label>
              <select id="species" required onchange="onSpeciesChange()"></select>
            </div>
            <div class="form-group">
              <label>Nivel (1 - 100)</label>
              <input type="number" id="level" min="1" max="100" value="50" required onchange="onLevelChange()">
            </div>
          </div>

          <div class="grid-2">
            <div class="form-group">
              <label>Mote (Nickname)</label>
              <input type="text" id="nickname" maxlength="10" value="GENGAR" oninput="updatePreview()">
            </div>
            <div class="form-group">
              <label>Entrenador Original (OT)</label>
              <input type="text" id="ot_name" maxlength="7" value="ESP32">
            </div>
          </div>

          <div class="grid-2">
            <div class="form-group">
              <label>ID Entrenador (OT ID)</label>
              <input type="number" id="ot_id" min="0" max="65535" value="42069">
            </div>
            <div class="form-group">
              <label>Presets IV / EV</label>
              <select id="eviv" onchange="updatePreview()">
                <option value="5" selected>Max IV & Max EV (Competitivo)</option>
                <option value="3">Max IV, Zero EV (Sin entrenar)</option>
                <option value="0">Random IV, Zero EV (Salvaje)</option>
                <option value="2">Random IV, Max EV</option>
              </select>
            </div>
          </div>

          <div style="display:flex; justify-content:space-between; align-items:center; margin:10px 0 6px 0;">
            <label style="margin:0; font-weight:600; font-size:0.85rem;">Movimientos (Ataques)</label>
            <span class="preset-pill" style="font-size:0.75rem; padding:3px 8px; margin:0;" onclick="applyDefaultMoves()">⚡ Restablecer ataques por nivel</span>
          </div>

          <div class="grid-2">
            <div class="form-group">
              <label>Ataque 1</label>
              <select id="m0"></select>
            </div>
            <div class="form-group">
              <label>Ataque 2</label>
              <select id="m1"></select>
            </div>
          </div>
          <div class="grid-2">
            <div class="form-group">
              <label>Ataque 3</label>
              <select id="m2"></select>
            </div>
            <div class="form-group">
              <label>Ataque 4</label>
              <select id="m3"></select>
            </div>
          </div>

          <div class="form-group" id="gen2Options" style="display:none; margin-top:8px;">
            <div class="switch-label">
              <span>✨ Variocolor / Shiny (Gen II)</span>
              <input type="checkbox" id="shiny" style="width:20px; height:20px;" onchange="updatePreview()">
            </div>
            <div class="form-group" style="margin-top:12px;">
              <label>Objeto Equipado (Gen II)</label>
              <select id="item"></select>
            </div>
          </div>

          <button type="submit" class="btn btn-primary" style="margin-top:10px;">
            💾 Cargar en la Mesa de Intercambio
          </button>
        </form>
      </div>
    </div>

    <!-- TAB 2: RECIBIDO -->
    <div id="tab-receive" class="tab-pane">
      <div class="card">
        <div class="card-title">📥 Pokémon Recibido de la Game Boy</div>
        <div id="noReceivedNotice" style="color:var(--text-muted); font-size:0.85rem; line-height: 1.5; padding: 12px 0;">
          Aún no se ha realizado un intercambio.<br>
          Cuando la Game Boy confirme el trato, el Pokémon que envíe tu consola aparecerá aquí para ser modificado.
        </div>
        
        <div id="receivedData" style="display:none;">
          <div class="pkmn-preview-box">
            <div class="pkmn-sprite">
              <img id="recSpriteImg" src="/sprites/000.png" alt="Received Pokemon" onerror="this.src='/sprites/000.png'">
            </div>
            <div>
              <div class="pkmn-summary-name">
                <span id="recSummaryName">POKÉMON</span>
                <span class="pkmn-summary-dex" id="recSummaryDex">#000</span>
              </div>
              <div class="pkmn-summary-details" id="recSummaryDetails">Recibido desde Game Boy</div>
            </div>
          </div>

          <div class="grid-2">
            <div class="form-group">
              <label>Especie</label>
              <input type="text" id="rec_species" readonly>
            </div>
            <div class="form-group">
              <label>Nivel Modificado</label>
              <input type="number" id="rec_level" min="1" max="100">
            </div>
          </div>
          <div class="form-group">
            <label>Mote</label>
            <input type="text" id="rec_nick" maxlength="10">
          </div>
          <div class="grid-2">
            <div class="form-group">
              <label>Ataque 1</label>
              <select id="rec_m0"></select>
            </div>
            <div class="form-group">
              <label>Ataque 2</label>
              <select id="rec_m1"></select>
            </div>
          </div>
          <div class="grid-2">
            <div class="form-group">
              <label>Ataque 3</label>
              <select id="rec_m2"></select>
            </div>
            <div class="form-group">
              <label>Ataque 4</label>
              <select id="rec_m3"></select>
            </div>
          </div>
          <div class="switch-label" style="margin: 14px 0;">
            <span>✨ Hacer Shiny al devolver a la Game Boy</span>
            <input type="checkbox" id="rec_shiny" style="width:20px; height:20px;">
          </div>
          <button type="button" class="btn btn-success" onclick="returnModifiedPokemon()">
            📤 Preparar para Devolver a Game Boy
          </button>
        </div>
      </div>
    </div>

    <!-- TAB 3: MONITOR -->
    <div id="tab-monitor" class="tab-pane">
      <div class="card">
        <div class="card-title">📡 Registro de Eventos del Cable Link</div>
        <div class="console-box" id="consoleLog">
          [12:00:00] Servidor web iniciado en 192.168.4.1<br>
          [12:00:00] Motor de trade en Core 1: CLK=GPIO4, SO=GPIO5, SI=GPIO6<br>
          [12:00:00] En espera de señal de reloj del Game Boy...<br>
        </div>
      </div>
    </div>

    <!-- TAB 4: AJUSTES -->
    <div id="tab-config" class="tab-pane">
      <div class="card">
        <div class="card-title">⚙️ Generación de Juego</div>
        <div class="form-group">
          <label>Cartucho Destino</label>
          <select id="genSelect" onchange="changeGeneration()">
            <option value="1" selected>Generación I (Rojo / Azul / Amarillo)</option>
            <option value="2">Generación II (Oro / Plata / Cristal)</option>
          </select>
        </div>
      </div>

      <div class="card">
        <div class="card-title">🔌 Esquema de Cableado (ESP32-S3 SuperMini)</div>
        <div style="font-size:0.82rem; color:var(--text-muted); line-height: 1.6;">
          • <strong>CLK (Pin 5 Game Boy):</strong> Conectar a <strong>GPIO 4</strong> con divisor de voltaje (1kΩ / 2kΩ) o Level Shifter.<br>
          • <strong>SO (Pin 2 Game Boy):</strong> Conectar a <strong>GPIO 5</strong> con divisor de voltaje.<br>
          • <strong>SI (Pin 3 Game Boy):</strong> Conectar directamente a <strong>GPIO 6</strong> (3.3V directo).<br>
          • <strong>GND (Pin 6 Game Boy):</strong> Conectar a <strong>GND</strong> del ESP32 (Masa común obligatoria).<br>
          • <strong>Pin 1 (VDD):</strong> ¡NO conectar! El ESP32 se alimenta por su puerto <strong>USB-C</strong>.
        </div>
      </div>
    </div>
  </div>

  <div class="toast" id="toast">¡Acción realizada con éxito!</div>

  <script>
    let currentGen = 1;

    const POKEMON_NAMES = ["Bulbasaur", "Ivysaur", "Venusaur", "Charmander", "Charmeleon", "Charizard", "Squirtle", "Wartortle", "Blastoise", "Caterpie", "Metapod", "Butterfree", "Weedle", "Kakuna", "Beedrill", "Pidgey", "Pidgeotto", "Pidgeot", "Rattata", "Raticate", "Spearow", "Fearow", "Ekans", "Arbok", "Pikachu", "Raichu", "Sandshrew", "Sandslash", "Nidoran\\200", "Nidorina", "Nidoqueen", "Nidoran\\201", "Nidorino", "Nidoking", "Clefairy", "Clefable", "Vulpix", "Ninetales", "Jigglypuff", "Wigglytuff", "Zubat", "Golbat", "Oddish", "Gloom", "Vileplume", "Paras", "Parasect", "Venonat", "Venomoth", "Diglett", "Dugtrio", "Meowth", "Persian", "Psyduck", "Golduck", "Mankey", "Primeape", "Growlithe", "Arcanine", "Poliwag", "Poliwhirl", "Poliwrath", "Abra", "Kadabra", "Alakazam", "Machop", "Machoke", "Machamp", "Bellsprout", "Weepinbell", "Victreebel", "Tentacool", "Tentacruel", "Geodude", "Graveler", "Golem", "Ponyta", "Rapidash", "Slowpoke", "Slowbro", "Magnemite", "Magneton", "Farfetch'd", "Doduo", "Dodrio", "Seel", "Dewgong", "Grimer", "Muk", "Shellder", "Cloyster", "Gastly", "Haunter", "Gengar", "Onix", "Drowzee", "Hypno", "Krabby", "Kingler", "Voltorb", "Electrode", "Exeggcute", "Exeggutor", "Cubone", "Marowak", "Hitmonlee", "Hitmonchan", "Lickitung", "Koffing", "Weezing", "Rhyhorn", "Rhydon", "Chansey", "Tangela", "Kangaskhan", "Horsea", "Seadra", "Goldeen", "Seaking", "Staryu", "Starmie", "Mr.Mime", "Scyther", "Jynx", "Electabuzz", "Magmar", "Pinsir", "Tauros", "Magikarp", "Gyarados", "Lapras", "Ditto", "Eevee", "Vaporeon", "Jolteon", "Flareon", "Porygon", "Omanyte", "Omastar", "Kabuto", "Kabutops", "Aerodactyl", "Snorlax", "Articuno", "Zapdos", "Moltres", "Dratini", "Dragonair", "Dragonite", "Mewtwo", "Mew", "Chikorita", "Bayleef", "Meganium", "Cyndaquil", "Quilava", "Typhlosion", "Totodile", "Croconaw", "Feraligatr", "Sentret", "Furret", "Hoothoot", "Noctowl", "Ledyba", "Ledian", "Spinarak", "Ariados", "Crobat", "Chinchou", "Lanturn", "Pichu", "Cleffa", "Igglybuff", "Togepi", "Togetic", "Natu", "Xatu", "Mareep", "Flaaffy", "Ampharos", "Bellossom", "Marill", "Azumarill", "Sudowoodo", "Politoed", "Hoppip", "Skiploom", "Jumpluff", "Aipom", "Sunkern", "Sunflora", "Yanma", "Wooper", "Quagsire", "Espeon", "Umbreon", "Murkrow", "Slowking", "Misdreavus", "Unown", "Wobbuffet", "Girafarig", "Pineco", "Forretress", "Dunsparce", "Gligar", "Steelix", "Snubbull", "Granbull", "Qwilfish", "Scizor", "Shuckle", "Heracross", "Sneasel", "Teddiursa", "Ursaring", "Slugma", "Magcargo", "Swinub", "Piloswine", "Corsola", "Remoraid", "Octillery", "Delibird", "Mantine", "Skarmory", "Houndour", "Houndoom", "Kingdra", "Phanpy", "Donphan", "Porygon2", "Stantler", "Smeargle", "Tyrogue", "Hitmontop", "Smoochum", "Elekid", "Magby", "Miltank", "Blissey", "Raikou", "Entei", "Suicune", "Larvitar", "Pupitar", "Tyranitar", "Lugia", "Ho-Oh", "Celebi"];
    const ALL_MOVES = [{"name": "No Move","id": 0,"gen": 1},{"name": "Absorb","id": 71,"gen": 1},{"name": "Acid","id": 51,"gen": 1},{"name": "Acid Armor","id": 151,"gen": 1},{"name": "Aeroblast","id": 177,"gen": 2},{"name": "Agility","id": 97,"gen": 1},{"name": "Amnesia","id": 133,"gen": 1},{"name": "Ancient Power","id": 246,"gen": 2},{"name": "Attract","id": 213,"gen": 2},{"name": "Aurora Beam","id": 62,"gen": 1},{"name": "Barrage","id": 140,"gen": 1},{"name": "Barrier","id": 112,"gen": 1},{"name": "Baton Pass","id": 226,"gen": 2},{"name": "Beat Up","id": 251,"gen": 2},{"name": "Belly Drum","id": 187,"gen": 2},{"name": "Bide","id": 117,"gen": 1},{"name": "Bind","id": 20,"gen": 1},{"name": "Bite","id": 44,"gen": 1},{"name": "Blizzard","id": 59,"gen": 1},{"name": "Body Slam","id": 34,"gen": 1},{"name": "Bone Club","id": 125,"gen": 1},{"name": "Bone Rush","id": 198,"gen": 2},{"name": "Boomerang","id": 155,"gen": 1},{"name": "Bubble","id": 145,"gen": 1},{"name": "Bubblebeam","id": 61,"gen": 1},{"name": "Charm","id": 204,"gen": 2},{"name": "Clamp","id": 128,"gen": 1},{"name": "Comet Punch","id": 4,"gen": 1},{"name": "Confuse Ray","id": 109,"gen": 1},{"name": "Confusion","id": 93,"gen": 1},{"name": "Constrict","id": 132,"gen": 1},{"name": "Conversion","id": 160,"gen": 1},{"name": "Conversion 2","id": 176,"gen": 2},{"name": "Cotton Spore","id": 178,"gen": 2},{"name": "Counter","id": 68,"gen": 1},{"name": "Crabhammer","id": 152,"gen": 1},{"name": "Cross Chop","id": 238,"gen": 2},{"name": "Crunch","id": 242,"gen": 2},{"name": "Curse","id": 174,"gen": 2},{"name": "Cut","id": 15,"gen": 1},{"name": "Defense Curl","id": 111,"gen": 1},{"name": "Destiny Bond","id": 194,"gen": 2},{"name": "Detect","id": 197,"gen": 2},{"name": "Dig","id": 91,"gen": 1},{"name": "Disable","id": 50,"gen": 1},{"name": "Dizzy Punch","id": 146,"gen": 1},{"name": "Double Kick","id": 24,"gen": 1},{"name": "Double Team","id": 104,"gen": 1},{"name": "Double-Edge","id": 38,"gen": 1},{"name": "Doubleslap","id": 3,"gen": 1},{"name": "Dragon Breath","id": 225,"gen": 2},{"name": "Dragon Rage","id": 82,"gen": 1},{"name": "Dream Eater","id": 138,"gen": 1},{"name": "Drill Peck","id": 65,"gen": 1},{"name": "Dynamic Punch","id": 223,"gen": 2},{"name": "Earthquake","id": 89,"gen": 1},{"name": "Egg Bomb","id": 121,"gen": 1},{"name": "Ember","id": 52,"gen": 1},{"name": "Encore","id": 227,"gen": 2},{"name": "Endure","id": 203,"gen": 2},{"name": "Explosion","id": 153,"gen": 1},{"name": "Extreme Speed","id": 245,"gen": 2},{"name": "False Swipe","id": 206,"gen": 2},{"name": "Feint Attack","id": 185,"gen": 2},{"name": "Fire Blast","id": 126,"gen": 1},{"name": "Fire Punch","id": 7,"gen": 1},{"name": "Fire Spin","id": 83,"gen": 1},{"name": "Fissure","id": 90,"gen": 1},{"name": "Flail","id": 175,"gen": 2},{"name": "Flame Wheel","id": 172,"gen": 2},{"name": "Flamethrower","id": 53,"gen": 1},{"name": "Flash","id": 148,"gen": 1},{"name": "Fly","id": 19,"gen": 1},{"name": "Focus Energy","id": 116,"gen": 1},{"name": "Foresight","id": 193,"gen": 2},{"name": "Frustration","id": 218,"gen": 2},{"name": "Fury Attack","id": 31,"gen": 1},{"name": "Fury Cutter","id": 210,"gen": 2},{"name": "Fury Swipes","id": 154,"gen": 1},{"name": "Future Sight","id": 248,"gen": 2},{"name": "Giga Drain","id": 202,"gen": 2},{"name": "Glare","id": 137,"gen": 1},{"name": "Growl","id": 45,"gen": 1},{"name": "Growth","id": 74,"gen": 1},{"name": "Guillotine","id": 12,"gen": 1},{"name": "Gust","id": 16,"gen": 1},{"name": "Harden","id": 106,"gen": 1},{"name": "Haze","id": 114,"gen": 1},{"name": "Headbutt","id": 29,"gen": 1},{"name": "Heal Bell","id": 215,"gen": 2},{"name": "Hi Jump Kick","id": 136,"gen": 1},{"name": "Hidden Power","id": 237,"gen": 2},{"name": "Horn Attack","id": 30,"gen": 1},{"name": "Horn Drill","id": 32,"gen": 1},{"name": "Hydro Pump","id": 56,"gen": 1},{"name": "Hyper Beam","id": 63,"gen": 1},{"name": "Hyper Fang","id": 158,"gen": 1},{"name": "Hypnosis","id": 95,"gen": 1},{"name": "Ice Beam","id": 58,"gen": 1},{"name": "Ice Punch","id": 8,"gen": 1},{"name": "Icy Wind","id": 196,"gen": 2},{"name": "Iron Tail","id": 231,"gen": 2},{"name": "Jump Kick","id": 26,"gen": 1},{"name": "Karate Chop","id": 2,"gen": 1},{"name": "Kinesis","id": 134,"gen": 1},{"name": "Leech Life","id": 141,"gen": 1},{"name": "Leech Seed","id": 73,"gen": 1},{"name": "Leer","id": 43,"gen": 1},{"name": "Lick","id": 122,"gen": 1},{"name": "Light Screen","id": 113,"gen": 1},{"name": "Lock-On","id": 199,"gen": 2},{"name": "Lovely Kiss","id": 142,"gen": 1},{"name": "Low Kick","id": 67,"gen": 1},{"name": "Mach Punch","id": 183,"gen": 2},{"name": "Magnitude","id": 222,"gen": 2},{"name": "Mean Look","id": 212,"gen": 2},{"name": "Meditate","id": 96,"gen": 1},{"name": "Mega Drain","id": 72,"gen": 1},{"name": "Mega Kick","id": 25,"gen": 1},{"name": "Mega Punch","id": 5,"gen": 1},{"name": "Megahorn","id": 224,"gen": 2},{"name": "Metal Claw","id": 232,"gen": 2},{"name": "Metronome","id": 118,"gen": 1},{"name": "Milk Drink","id": 208,"gen": 2},{"name": "Mimic","id": 102,"gen": 1},{"name": "Mind Reader","id": 170,"gen": 2},{"name": "Minimize","id": 107,"gen": 1},{"name": "Mirror Coat","id": 243,"gen": 2},{"name": "Mirror Move","id": 119,"gen": 1},{"name": "Mist","id": 54,"gen": 1},{"name": "Moonlight","id": 236,"gen": 2},{"name": "Morning Sun","id": 234,"gen": 2},{"name": "Mud-Slap","id": 189,"gen": 2},{"name": "Night Shade","id": 101,"gen": 1},{"name": "Nightmare","id": 171,"gen": 2},{"name": "Octazooka","id": 190,"gen": 2},{"name": "Outrage","id": 200,"gen": 2},{"name": "Pain Split","id": 220,"gen": 2},{"name": "Pay Day","id": 6,"gen": 1},{"name": "Peck","id": 64,"gen": 1},{"name": "Perish Song","id": 195,"gen": 2},{"name": "Petal Dance","id": 80,"gen": 1},{"name": "Pin Missile","id": 42,"gen": 1},{"name": "Poison Gas","id": 139,"gen": 1},{"name": "Poison Sting","id": 40,"gen": 1},{"name": "Poisonpowder","id": 77,"gen": 1},{"name": "Pound","id": 1,"gen": 1},{"name": "Powder Snow","id": 181,"gen": 2},{"name": "Present","id": 217,"gen": 2},{"name": "Protect","id": 182,"gen": 2},{"name": "Psybeam","id": 60,"gen": 1},{"name": "Psych Up","id": 244,"gen": 2},{"name": "Psychic","id": 94,"gen": 1},{"name": "Psywave","id": 149,"gen": 1},{"name": "Pursuit","id": 228,"gen": 2},{"name": "Quick Attack","id": 98,"gen": 1},{"name": "Rage","id": 99,"gen": 1},{"name": "Rain Dance","id": 240,"gen": 2},{"name": "Rapid Spin","id": 229,"gen": 2},{"name": "Razor Leaf","id": 75,"gen": 1},{"name": "Razor Wind","id": 13,"gen": 1},{"name": "Recover","id": 105,"gen": 1},{"name": "Reflect","id": 115,"gen": 1},{"name": "Rest","id": 156,"gen": 1},{"name": "Return","id": 216,"gen": 2},{"name": "Reversal","id": 179,"gen": 2},{"name": "Roar","id": 46,"gen": 1},{"name": "Rock Slide","id": 157,"gen": 1},{"name": "Rock Smash","id": 249,"gen": 2},{"name": "Rock Throw","id": 88,"gen": 1},{"name": "Rolling Kick","id": 27,"gen": 1},{"name": "Rollout","id": 205,"gen": 2},{"name": "Sacred Fire","id": 221,"gen": 2},{"name": "Safeguard","id": 219,"gen": 2},{"name": "Sand Attack","id": 28,"gen": 1},{"name": "Sandstorm","id": 201,"gen": 2},{"name": "Scary Face","id": 184,"gen": 2},{"name": "Scratch","id": 10,"gen": 1},{"name": "Screech","id": 103,"gen": 1},{"name": "Seismic Toss","id": 69,"gen": 1},{"name": "Selfdestruct","id": 120,"gen": 1},{"name": "Shadow Ball","id": 247,"gen": 2},{"name": "Sharpen","id": 159,"gen": 1},{"name": "Sing","id": 47,"gen": 1},{"name": "Sketch","id": 166,"gen": 2},{"name": "Skull Bash","id": 130,"gen": 1},{"name": "Sky Attack","id": 143,"gen": 1},{"name": "Slam","id": 21,"gen": 1},{"name": "Slash","id": 163,"gen": 1},{"name": "Sleep Powder","id": 79,"gen": 1},{"name": "Sleep Talk","id": 214,"gen": 2},{"name": "Sludge","id": 124,"gen": 1},{"name": "Sludge Bomb","id": 188,"gen": 2},{"name": "Smog","id": 123,"gen": 1},{"name": "Smokescreen","id": 108,"gen": 1},{"name": "Snore","id": 173,"gen": 2},{"name": "Softboiled","id": 135,"gen": 1},{"name": "Solar Beam","id": 76,"gen": 1},{"name": "Sonicboom","id": 49,"gen": 1},{"name": "Spark","id": 209,"gen": 2},{"name": "Spider Web","id": 169,"gen": 2},{"name": "Spike Cannon","id": 131,"gen": 1},{"name": "Spikes","id": 191,"gen": 2},{"name": "Spite","id": 180,"gen": 2},{"name": "Splash","id": 150,"gen": 1},{"name": "Spore","id": 147,"gen": 1},{"name": "Steel Wing","id": 211,"gen": 2},{"name": "Stomp","id": 23,"gen": 1},{"name": "Strength","id": 70,"gen": 1},{"name": "String Shot","id": 81,"gen": 1},{"name": "Struggle","id": 165,"gen": 1},{"name": "Stun Spore","id": 78,"gen": 1},{"name": "Submission","id": 66,"gen": 1},{"name": "Substitute","id": 164,"gen": 1},{"name": "Sunny Day","id": 241,"gen": 2},{"name": "Super Fang","id": 162,"gen": 1},{"name": "Supersonic","id": 48,"gen": 1},{"name": "Surf","id": 57,"gen": 1},{"name": "Swagger","id": 207,"gen": 2},{"name": "Sweet Kiss","id": 186,"gen": 2},{"name": "Sweet Scent","id": 230,"gen": 2},{"name": "Swift","id": 129,"gen": 1},{"name": "Swords Dance","id": 14,"gen": 1},{"name": "Synthesis","id": 235,"gen": 2},{"name": "Tackle","id": 33,"gen": 1},{"name": "Tail Whip","id": 39,"gen": 1},{"name": "Take Down","id": 36,"gen": 1},{"name": "Teleport","id": 100,"gen": 1},{"name": "Thief","id": 168,"gen": 2},{"name": "Thrash","id": 37,"gen": 1},{"name": "Thunder","id": 87,"gen": 1},{"name": "Thunder Wave","id": 86,"gen": 1},{"name": "Thunderbolt","id": 85,"gen": 1},{"name": "Thunderpunch","id": 9,"gen": 1},{"name": "Thundershock","id": 84,"gen": 1},{"name": "Toxic","id": 92,"gen": 1},{"name": "Transform","id": 144,"gen": 1},{"name": "Tri Attack","id": 161,"gen": 1},{"name": "Triple Kick","id": 167,"gen": 2},{"name": "Twineedle","id": 41,"gen": 1},{"name": "Twister","id": 239,"gen": 2},{"name": "Vicegrip","id": 11,"gen": 1},{"name": "Vine Whip","id": 22,"gen": 1},{"name": "Vital Throw","id": 233,"gen": 2},{"name": "Water Gun","id": 55,"gen": 1},{"name": "Waterfall","id": 127,"gen": 1},{"name": "Whirlpool","id": 250,"gen": 2},{"name": "Whirlwind","id": 18,"gen": 1},{"name": "Wing Attack","id": 17,"gen": 1},{"name": "Withdraw","id": 110,"gen": 1},{"name": "Wrap","id": 35,"gen": 1},{"name": "Zap Cannon","id": 192,"gen": 2}];
    const MOVE_NAME_TO_ID = {};
    const MOVE_ID_TO_NAME = {};
    ALL_MOVES.forEach(m => {
      MOVE_NAME_TO_ID[m.name.toLowerCase()] = m.id;
      MOVE_ID_TO_NAME[m.id] = m.name;
    });

    function moveIdToName(id) {
      if (id === 0 || id === undefined || id === null) return "No Move";
      return MOVE_ID_TO_NAME[id] || ("Move #" + id);
    }

    function moveToId(val) {
      if (!val) return 0;
      if (typeof val === "number") return (val >= 0 && val <= 255) ? val : 0;
      const trimmed = String(val).trim();
      if (!trimmed) return 0;
      if (/^\d+$/.test(trimmed)) {
        const num = parseInt(trimmed, 10);
        return (num >= 0 && num <= 255) ? num : 0;
      }
      const id = MOVE_NAME_TO_ID[trimmed.toLowerCase()];
      return (id !== undefined) ? id : 0;
    }

    function populateMovesSelect(gen) {
      const moveSelectIds = ['m0', 'm1', 'm2', 'm3', 'rec_m0', 'rec_m1', 'rec_m2', 'rec_m3'];
      moveSelectIds.forEach(id => {
        const sel = document.getElementById(id);
        if (!sel) return;
        const prev = parseInt(sel.value) || 0;
        sel.innerHTML = "";
        ALL_MOVES.forEach(m => {
          if (gen === 1 && m.gen !== 1) return;
          const opt = document.createElement("option");
          opt.value = m.id;
          opt.textContent = m.id === 0 ? "No Move" : `${m.name} (#${m.id})`;
          if (m.id === prev) opt.selected = true;
          sel.appendChild(opt);
        });
        if (prev !== 0) {
          const valid = ALL_MOVES.find(m => m.id === prev && (gen !== 1 || m.gen === 1));
          sel.value = valid ? prev : 0;
        }
      });
    }

    const POKEMON_LEARNSETS = {"1": [1, 33, 4, 45, 7, 73, 10, 22, 15, 77, 15, 79, 20, 75, 25, 230, 32, 74, 39, 235, 46, 76], "2": [1, 33, 1, 45, 1, 73, 4, 45, 7, 73, 10, 22, 15, 77, 15, 79, 22, 75, 29, 230, 38, 74, 47, 235, 56, 76], "3": [1, 33, 1, 45, 1, 73, 1, 22, 4, 45, 7, 73, 10, 22, 15, 77, 15, 79, 22, 75, 29, 230, 41, 74, 53, 235, 65, 76], "4": [1, 10, 1, 45, 7, 52, 13, 108, 19, 99, 25, 184, 31, 53, 37, 163, 43, 82, 49, 83], "5": [1, 10, 1, 45, 1, 52, 7, 52, 13, 108, 20, 99, 27, 184, 34, 53, 41, 163, 48, 82, 55, 83], "6": [1, 10, 1, 45, 1, 52, 1, 108, 7, 52, 13, 108, 20, 99, 27, 184, 34, 53, 36, 17, 44, 163, 54, 82, 64, 83], "7": [1, 33, 4, 39, 7, 145, 10, 110, 13, 55, 18, 44, 23, 229, 28, 182, 33, 240, 40, 130, 47, 56], "8": [1, 33, 1, 39, 1, 145, 4, 39, 7, 145, 10, 110, 13, 55, 19, 44, 25, 229, 31, 182, 37, 240, 45, 130, 53, 56], "9": [1, 33, 1, 39, 1, 145, 1, 110, 4, 39, 7, 145, 10, 110, 13, 55, 19, 44, 25, 229, 31, 182, 42, 240, 55, 130, 68, 56], "10": [1, 33, 1, 81], "11": [1, 106, 7, 106], "12": [1, 93, 10, 93, 13, 77, 14, 78, 15, 79, 18, 48, 23, 18, 28, 16, 34, 60, 40, 219], "13": [1, 40, 1, 81], "14": [1, 106, 7, 106], "15": [1, 31, 10, 31, 15, 116, 20, 41, 25, 99, 30, 228, 35, 42, 40, 97], "16": [1, 33, 5, 28, 9, 16, 15, 98, 21, 18, 29, 17, 37, 97, 47, 119], "17": [1, 33, 1, 28, 1, 16, 5, 28, 9, 16, 15, 98, 23, 18, 33, 17, 43, 97, 55, 119], "18": [1, 33, 1, 28, 1, 16, 1, 98, 5, 28, 9, 16, 15, 98, 23, 18, 33, 17, 46, 97, 61, 119], "19": [1, 33, 1, 39, 7, 98, 13, 158, 20, 116, 27, 228, 34, 162], "20": [1, 33, 1, 39, 1, 98, 7, 98, 13, 158, 20, 184, 30, 228, 40, 162], "21": [1, 64, 1, 45, 7, 43, 13, 31, 25, 228, 31, 119, 37, 65, 43, 97], "22": [1, 64, 1, 45, 1, 43, 1, 31, 7, 43, 13, 31, 26, 228, 32, 119, 40, 65, 47, 97], "23": [1, 35, 1, 43, 9, 40, 15, 44, 23, 137, 29, 103, 37, 51, 43, 114], "24": [1, 35, 1, 43, 1, 40, 1, 44, 9, 40, 15, 44, 25, 137, 33, 103, 43, 51, 51, 114], "25": [1, 84, 1, 45, 6, 39, 8, 86, 11, 98, 15, 104, 20, 21, 26, 85, 33, 97, 41, 87, 50, 113], "26": [1, 84, 1, 39, 1, 98, 1, 85], "27": [1, 10, 6, 111, 11, 28, 17, 40, 23, 163, 30, 129, 37, 154, 45, 201], "28": [1, 10, 1, 111, 1, 28, 6, 111, 11, 28, 17, 40, 24, 163, 33, 129, 42, 154, 52, 201], "29": [1, 45, 1, 33, 8, 10, 12, 24, 17, 40, 23, 39, 30, 44, 38, 154], "30": [1, 45, 1, 33, 8, 10, 12, 24, 19, 40, 27, 39, 36, 44, 46, 154], "31": [1, 33, 1, 10, 1, 24, 1, 39, 23, 34], "32": [1, 43, 1, 33, 8, 30, 12, 24, 17, 40, 23, 116, 30, 31, 38, 32], "33": [1, 43, 1, 33, 8, 30, 12, 24, 19, 40, 27, 116, 36, 31, 46, 32], "34": [1, 33, 1, 30, 1, 24, 1, 40, 23, 37], "35": [1, 1, 1, 45, 4, 227, 8, 47, 13, 3, 19, 107, 26, 111, 34, 118, 43, 236, 53, 113], "36": [1, 47, 1, 3, 1, 118, 1, 236], "37": [1, 52, 1, 39, 7, 98, 13, 46, 19, 109, 25, 219, 31, 53, 37, 83], "38": [1, 52, 1, 98, 1, 109, 1, 219, 43, 83], "39": [1, 47, 4, 111, 9, 1, 14, 50, 19, 205, 24, 3, 29, 156, 34, 34, 39, 38], "40": [1, 47, 1, 50, 1, 111, 1, 3], "41": [1, 141, 6, 48, 12, 44, 19, 109, 27, 17, 36, 212, 46, 114], "42": [1, 103, 1, 141, 1, 48, 6, 48, 12, 44, 19, 109, 30, 17, 42, 212, 55, 114], "43": [1, 71, 7, 230, 14, 77, 16, 78, 18, 79, 23, 51, 32, 236, 39, 80], "44": [1, 71, 1, 230, 1, 77, 7, 230, 14, 77, 16, 78, 18, 79, 24, 51, 35, 236, 44, 80], "45": [1, 71, 1, 230, 1, 78, 1, 80], "46": [1, 10, 7, 78, 13, 77, 19, 141, 25, 147, 31, 163, 37, 74, 43, 202], "47": [1, 10, 1, 78, 1, 77, 7, 78, 13, 77, 19, 141, 28, 147, 37, 163, 46, 74, 55, 202], "48": [1, 33, 1, 50, 1, 193, 9, 48, 17, 93, 20, 77, 25, 141, 28, 78, 33, 60, 36, 79, 41, 94], "49": [1, 33, 1, 50, 1, 193, 1, 48, 9, 48, 17, 93, 20, 77, 25, 141, 28, 78, 31, 16, 36, 60, 42, 79, 52, 94], "50": [1, 10, 5, 45, 9, 222, 17, 91, 25, 28, 33, 163, 41, 89, 49, 90], "51": [1, 10, 1, 45, 1, 222, 5, 45, 9, 222, 17, 91, 25, 28, 37, 163, 49, 89, 61, 90], "52": [1, 10, 1, 45, 11, 44, 20, 6, 28, 185, 35, 103, 41, 154, 46, 163], "53": [1, 10, 1, 45, 1, 44, 11, 44, 20, 6, 29, 185, 38, 103, 46, 154, 53, 163], "54": [1, 10, 5, 39, 10, 50, 16, 93, 23, 103, 31, 244, 40, 154, 50, 56], "55": [1, 10, 1, 39, 1, 50, 1, 93, 5, 39, 10, 50, 16, 93, 23, 103, 31, 244, 44, 154, 58, 56], "56": [1, 10, 1, 43, 9, 67, 15, 2, 21, 154, 27, 116, 33, 69, 39, 238, 45, 103, 51, 37], "57": [1, 10, 1, 43, 1, 67, 1, 99, 9, 67, 15, 2, 21, 154, 27, 116, 28, 99, 36, 69, 45, 238, 54, 103, 63, 37], "58": [1, 44, 1, 46, 9, 52, 18, 43, 26, 36, 34, 172, 42, 97, 50, 53], "59": [1, 46, 1, 43, 1, 36, 1, 172, 50, 245], "60": [1, 145, 7, 95, 13, 55, 19, 3, 25, 240, 31, 34, 37, 187, 43, 56], "61": [1, 145, 1, 95, 1, 55, 7, 95, 13, 55, 19, 3, 27, 240, 35, 34, 43, 187, 51, 56], "62": [1, 55, 1, 95, 1, 3, 1, 66, 35, 66, 51, 170], "63": [1, 100], "64": [1, 100, 1, 134, 1, 93, 16, 93, 18, 50, 21, 60, 26, 105, 31, 248, 38, 94, 45, 115], "65": [1, 100, 1, 134, 1, 93, 16, 93, 18, 50, 21, 60, 26, 105, 31, 248, 38, 94, 45, 115], "66": [1, 67, 1, 43, 7, 116, 13, 2, 19, 69, 25, 193, 31, 233, 37, 238, 43, 184, 49, 66], "67": [1, 67, 1, 43, 1, 116, 8, 116, 15, 2, 19, 69, 25, 193, 34, 233, 43, 238, 52, 184, 61, 66], "68": [1, 67, 1, 43, 1, 116, 8, 116, 15, 2, 19, 69, 25, 193, 34, 233, 43, 238, 52, 184, 61, 66], "69": [1, 22, 6, 74, 11, 35, 15, 79, 17, 77, 19, 78, 23, 51, 30, 230, 37, 75, 45, 21], "70": [1, 22, 1, 74, 1, 35, 6, 74, 11, 35, 15, 79, 17, 77, 19, 78, 24, 51, 33, 230, 42, 75, 54, 21], "71": [1, 22, 1, 79, 1, 230, 1, 75], "72": [1, 40, 6, 48, 12, 132, 19, 51, 25, 61, 30, 35, 36, 112, 43, 103, 49, 56], "73": [1, 40, 1, 48, 1, 132, 6, 48, 12, 132, 19, 51, 25, 61, 30, 35, 38, 112, 47, 103, 55, 56], "74": [1, 33, 6, 111, 11, 88, 16, 222, 21, 120, 26, 106, 31, 205, 36, 89, 41, 153], "75": [1, 33, 1, 111, 1, 88, 6, 111, 11, 88, 16, 222, 21, 120, 27, 106, 34, 205, 41, 89, 48, 153], "76": [1, 33, 1, 111, 1, 88, 1, 222, 6, 111, 11, 88, 16, 222, 21, 120, 27, 106, 34, 205, 41, 89, 48, 153], "77": [1, 33, 4, 45, 8, 39, 13, 52, 19, 23, 26, 83, 34, 36, 43, 97, 53, 126], "78": [1, 33, 1, 45, 1, 39, 1, 52, 4, 45, 8, 39, 13, 52, 19, 23, 26, 83, 34, 36, 40, 31, 47, 97, 61, 126], "79": [1, 174, 1, 33, 6, 45, 15, 55, 20, 93, 29, 50, 34, 29, 43, 133, 48, 94], "80": [1, 174, 1, 33, 1, 45, 1, 55, 6, 45, 15, 55, 20, 93, 29, 50, 34, 29, 37, 110, 46, 133, 54, 94], "81": [1, 33, 6, 84, 11, 48, 16, 49, 21, 86, 27, 199, 33, 129, 39, 103, 45, 192], "82": [1, 33, 1, 84, 1, 48, 1, 49, 6, 84, 11, 48, 16, 49, 21, 86, 27, 199, 35, 129, 43, 103, 53, 192], "83": [1, 64, 7, 28, 13, 43, 19, 31, 25, 14, 31, 97, 37, 163, 44, 206], "84": [1, 64, 1, 45, 9, 228, 13, 31, 21, 161, 25, 99, 33, 65, 37, 97], "85": [1, 64, 1, 45, 1, 228, 1, 31, 9, 228, 13, 31, 21, 161, 25, 99, 38, 65, 47, 97], "86": [1, 29, 5, 45, 16, 62, 21, 156, 32, 36, 37, 58, 48, 219], "87": [1, 29, 1, 45, 1, 62, 5, 45, 16, 62, 21, 156, 32, 36, 43, 58, 60, 219], "88": [1, 139, 1, 1, 5, 106, 10, 50, 16, 124, 23, 107, 31, 103, 40, 151, 50, 188], "89": [1, 139, 1, 1, 1, 106, 33, 106, 37, 50, 45, 124, 23, 107, 31, 103, 45, 151, 60, 188], "90": [1, 33, 1, 110, 9, 48, 17, 62, 25, 182, 33, 43, 41, 128, 49, 58], "91": [1, 110, 1, 48, 1, 62, 1, 182, 41, 131], "92": [1, 95, 1, 122, 8, 180, 13, 212, 16, 174, 21, 101, 28, 109, 33, 138, 36, 194], "93": [1, 95, 1, 122, 1, 180, 8, 180, 13, 212, 16, 174, 21, 101, 31, 109, 39, 138, 48, 194], "94": [1, 95, 1, 122, 1, 180, 8, 180, 13, 212, 16, 174, 21, 101, 31, 109, 39, 138, 48, 194], "95": [1, 33, 1, 103, 10, 20, 14, 88, 23, 106, 27, 99, 36, 201, 40, 21], "96": [1, 1, 1, 95, 10, 50, 18, 93, 25, 29, 31, 139, 36, 96, 40, 94, 43, 244, 45, 248], "97": [1, 1, 1, 95, 1, 50, 1, 93, 10, 50, 18, 93, 25, 29, 33, 139, 40, 96, 49, 94, 55, 244, 60, 248], "98": [1, 145, 5, 43, 12, 11, 16, 106, 23, 23, 27, 12, 34, 182, 41, 152], "99": [1, 145, 1, 43, 1, 11, 5, 43, 12, 11, 16, 106, 23, 23, 27, 12, 38, 182, 49, 152], "100": [1, 33, 9, 103, 17, 49, 23, 120, 29, 205, 33, 113, 37, 129, 39, 153, 41, 243], "101": [1, 33, 1, 103, 1, 49, 1, 120, 9, 103, 17, 49, 23, 120, 29, 205, 34, 113, 40, 129, 44, 153, 48, 243], "102": [1, 140, 1, 95, 7, 115, 13, 73, 19, 93, 25, 78, 31, 77, 37, 79, 43, 76], "103": [1, 140, 1, 95, 1, 93, 19, 23, 31, 121], "104": [1, 45, 5, 39, 9, 125, 13, 29, 17, 43, 21, 116, 25, 155, 29, 99, 33, 206, 37, 37, 41, 198], "105": [1, 45, 1, 39, 1, 125, 1, 29, 5, 39, 9, 125, 13, 29, 17, 43, 21, 116, 25, 155, 32, 99, 39, 206, 46, 37, 53, 198], "106": [1, 24, 6, 96, 11, 27, 16, 26, 21, 116, 26, 136, 31, 170, 36, 193, 41, 203, 46, 25, 51, 179], "107": [1, 4, 7, 97, 13, 228, 26, 9, 26, 8, 26, 7, 32, 183, 38, 5, 44, 197, 50, 68], "108": [1, 122, 7, 48, 13, 111, 19, 23, 25, 35, 31, 50, 37, 21, 43, 103], "109": [1, 139, 1, 33, 9, 123, 17, 120, 21, 124, 25, 108, 33, 114, 41, 153, 45, 194], "110": [1, 139, 1, 33, 1, 123, 1, 120, 9, 123, 17, 120, 21, 124, 25, 108, 33, 114, 44, 153, 51, 194], "111": [1, 30, 1, 39, 13, 23, 19, 31, 31, 184, 37, 32, 49, 36, 55, 89], "112": [1, 30, 1, 39, 1, 23, 1, 31, 13, 23, 19, 31, 31, 184, 37, 32, 54, 36, 65, 89], "113": [1, 1, 5, 45, 9, 39, 13, 135, 17, 3, 23, 107, 29, 47, 35, 121, 41, 111, 49, 113, 57, 38], "114": [1, 132, 4, 79, 10, 71, 13, 77, 19, 22, 25, 20, 31, 72, 34, 78, 40, 21, 46, 74], "115": [1, 4, 7, 43, 13, 44, 19, 39, 25, 5, 31, 99, 37, 203, 43, 146, 49, 179], "116": [1, 145, 8, 108, 15, 43, 22, 55, 29, 239, 36, 97, 43, 56], "117": [1, 145, 1, 108, 1, 43, 1, 55, 8, 108, 15, 43, 22, 55, 29, 239, 40, 97, 51, 56], "118": [1, 64, 1, 39, 10, 48, 15, 30, 24, 175, 29, 31, 38, 127, 43, 32, 52, 97], "119": [1, 64, 1, 39, 1, 39, 10, 48, 15, 30, 24, 175, 29, 31, 41, 127, 49, 32, 61, 97], "120": [1, 33, 1, 106, 7, 55, 13, 229, 19, 105, 25, 129, 31, 61, 37, 107, 43, 113, 50, 56], "121": [1, 33, 1, 229, 1, 105, 1, 61, 37, 109], "122": [1, 112, 6, 93, 11, 164, 16, 96, 21, 3, 26, 113, 26, 115, 31, 227, 36, 60, 41, 226, 46, 219], "123": [1, 98, 1, 43, 6, 116, 12, 228, 18, 206, 24, 97, 30, 17, 36, 163, 42, 14, 48, 104], "124": [1, 1, 1, 122, 1, 142, 1, 181, 9, 142, 13, 181, 21, 3, 25, 8, 35, 212, 41, 34, 51, 195, 57, 59], "125": [1, 98, 1, 43, 1, 9, 9, 9, 17, 113, 25, 129, 36, 103, 47, 85, 58, 87], "126": [1, 52, 1, 43, 1, 123, 1, 7, 7, 43, 13, 123, 19, 7, 25, 108, 33, 241, 41, 53, 49, 109, 57, 126], "127": [1, 11, 7, 116, 13, 20, 19, 69, 25, 106, 31, 12, 37, 66, 43, 14], "128": [1, 33, 4, 39, 8, 99, 13, 30, 19, 184, 26, 228, 34, 156, 43, 37, 53, 36], "129": [1, 150, 15, 33, 30, 175], "130": [1, 37, 20, 44, 25, 82, 30, 43, 35, 239, 40, 56, 45, 240, 50, 63], "131": [1, 55, 1, 45, 1, 47, 8, 54, 15, 34, 22, 109, 29, 195, 36, 58, 43, 240, 50, 219, 57, 56], "132": [1, 144], "133": [1, 33, 1, 39, 8, 28, 16, 45, 23, 98, 30, 44, 36, 116, 42, 36], "134": [1, 33, 1, 39, 8, 28, 16, 55, 23, 98, 30, 44, 36, 62, 42, 114, 47, 151, 52, 56], "135": [1, 33, 1, 39, 8, 28, 16, 84, 23, 98, 30, 24, 36, 42, 42, 86, 47, 97, 52, 87], "136": [1, 33, 1, 39, 8, 28, 16, 52, 23, 98, 30, 44, 36, 83, 42, 123, 47, 43, 52, 53], "137": [1, 176, 1, 33, 1, 160, 9, 97, 12, 60, 20, 105, 24, 159, 32, 199, 36, 161, 44, 192], "138": [1, 132, 1, 110, 13, 44, 19, 55, 31, 43, 37, 182, 49, 246, 55, 56], "139": [1, 132, 1, 110, 1, 44, 13, 44, 19, 55, 31, 43, 37, 182, 40, 131, 54, 246, 65, 56], "140": [1, 10, 1, 106, 10, 71, 19, 43, 28, 28, 37, 203, 46, 72, 55, 246], "141": [1, 10, 1, 106, 1, 71, 10, 71, 19, 43, 28, 28, 37, 203, 40, 163, 51, 72, 65, 246], "142": [1, 17, 8, 97, 15, 44, 22, 48, 29, 246, 36, 184, 43, 36, 50, 63], "143": [1, 33, 8, 133, 15, 111, 22, 187, 29, 29, 36, 173, 36, 156, 43, 34, 50, 205, 57, 63], "144": [1, 16, 1, 181, 13, 54, 25, 97, 37, 170, 49, 58, 61, 115, 73, 59], "145": [1, 64, 1, 84, 13, 86, 25, 97, 37, 197, 49, 65, 61, 113, 73, 87], "146": [1, 17, 1, 52, 13, 83, 25, 97, 37, 203, 49, 53, 61, 219, 73, 143], "147": [1, 35, 1, 43, 8, 86, 15, 239, 22, 82, 29, 21, 36, 97, 43, 219, 50, 200, 57, 63], "148": [1, 35, 1, 43, 1, 86, 1, 239, 8, 86, 15, 239, 22, 82, 29, 21, 38, 97, 47, 219, 56, 200, 65, 63], "149": [1, 35, 1, 43, 1, 86, 1, 239, 8, 86, 15, 239, 22, 82, 29, 21, 38, 97, 47, 219, 55, 17, 61, 200, 75, 63], "150": [1, 93, 1, 50, 11, 112, 22, 129, 33, 244, 44, 248, 55, 54, 66, 94, 77, 133, 88, 105, 99, 219], "151": [1, 1, 10, 144, 20, 5, 30, 118, 40, 94, 50, 246], "152": [1, 33, 1, 45, 8, 75, 12, 115, 15, 77, 22, 235, 29, 34, 36, 113, 43, 219, 50, 76], "153": [1, 33, 1, 45, 1, 75, 1, 115, 8, 75, 12, 115, 15, 77, 23, 235, 31, 34, 39, 113, 47, 219, 55, 76], "154": [1, 33, 1, 45, 1, 75, 1, 115, 8, 75, 12, 115, 15, 77, 23, 235, 31, 34, 41, 113, 51, 219, 61, 76], "155": [1, 33, 1, 43, 6, 108, 12, 52, 19, 98, 27, 172, 36, 129, 46, 53], "156": [1, 33, 1, 43, 1, 108, 6, 108, 12, 52, 21, 98, 31, 172, 42, 129, 54, 53], "157": [1, 33, 1, 43, 1, 108, 1, 52, 6, 108, 12, 52, 21, 98, 31, 172, 45, 129, 60, 53], "158": [1, 10, 1, 43, 7, 99, 13, 55, 20, 44, 27, 184, 35, 163, 43, 103, 52, 56], "159": [1, 10, 1, 43, 1, 99, 7, 99, 13, 55, 21, 44, 28, 184, 37, 163, 45, 103, 55, 56], "160": [1, 10, 1, 43, 1, 99, 1, 55, 7, 99, 13, 55, 21, 44, 28, 184, 38, 163, 47, 103, 58, 56], "161": [1, 33, 5, 111, 11, 98, 17, 154, 25, 21, 33, 156, 41, 133], "162": [1, 10, 1, 111, 1, 98, 5, 111, 11, 98, 18, 154, 28, 21, 38, 156, 48, 133], "163": [1, 33, 1, 45, 6, 193, 11, 64, 16, 95, 22, 115, 28, 36, 34, 93, 48, 138], "164": [1, 33, 1, 45, 1, 193, 1, 64, 6, 193, 11, 64, 16, 95, 25, 115, 33, 36, 41, 93, 57, 138], "165": [1, 33, 8, 48, 15, 4, 22, 113, 22, 115, 22, 219, 29, 226, 36, 129, 43, 97, 50, 38], "166": [1, 33, 1, 48, 8, 48, 15, 4, 24, 113, 24, 115, 24, 219, 33, 226, 42, 129, 51, 97, 60, 38], "167": [1, 40, 1, 81, 6, 184, 11, 132, 17, 101, 23, 141, 30, 154, 37, 169, 45, 103, 53, 94], "168": [1, 40, 1, 81, 1, 184, 1, 132, 6, 184, 11, 132, 17, 101, 25, 141, 34, 154, 43, 169, 53, 103, 63, 94], "169": [1, 103, 1, 141, 1, 48, 6, 48, 12, 44, 19, 109, 30, 17, 42, 212, 55, 114], "170": [1, 145, 1, 86, 5, 48, 13, 175, 17, 55, 25, 209, 29, 109, 37, 36, 41, 56], "171": [1, 145, 1, 86, 1, 48, 5, 48, 13, 175, 17, 55, 25, 209, 33, 109, 45, 36, 53, 56], "172": [1, 84, 1, 204, 6, 39, 8, 86, 11, 186], "173": [1, 1, 1, 204, 4, 227, 8, 47, 13, 186], "174": [1, 47, 1, 204, 4, 111, 9, 1, 14, 186], "175": [1, 45, 1, 204, 7, 118, 18, 186, 25, 227, 31, 219, 38, 38], "176": [1, 45, 1, 204, 7, 118, 18, 186, 25, 227, 31, 219, 38, 38], "177": [1, 64, 1, 43, 10, 101, 20, 100, 30, 248, 40, 109, 50, 94], "178": [1, 64, 1, 43, 1, 101, 10, 101, 20, 100, 35, 248, 50, 109, 65, 94], "179": [1, 33, 1, 45, 9, 84, 16, 86, 23, 178, 30, 113, 37, 87], "180": [1, 33, 1, 45, 1, 84, 9, 84, 18, 86, 27, 178, 36, 113, 45, 87], "181": [1, 33, 1, 45, 1, 84, 1, 86, 9, 84, 18, 86, 27, 178, 30, 9, 42, 113, 57, 87], "182": [1, 71, 1, 230, 1, 78, 1, 80, 55, 76], "183": [1, 33, 3, 111, 6, 39, 10, 55, 15, 205, 21, 61, 28, 38, 36, 240], "184": [1, 33, 1, 111, 1, 39, 1, 55, 3, 111, 6, 39, 10, 55, 15, 205, 25, 61, 36, 38, 48, 240], "185": [1, 88, 1, 102, 10, 175, 19, 67, 28, 157, 37, 185, 46, 21], "186": [1, 55, 1, 95, 1, 3, 1, 195, 35, 195, 51, 207], "187": [1, 150, 1, 235, 5, 39, 10, 33, 13, 77, 15, 78, 17, 79, 20, 73, 25, 178, 30, 72], "188": [1, 150, 1, 235, 1, 39, 1, 33, 5, 39, 10, 33, 13, 77, 15, 78, 17, 79, 22, 73, 29, 178, 36, 72], "189": [1, 150, 1, 235, 1, 39, 1, 33, 5, 39, 10, 33, 13, 77, 15, 78, 17, 79, 22, 73, 33, 178, 44, 72], "190": [1, 10, 1, 39, 6, 28, 12, 226, 19, 154, 27, 129, 36, 103, 46, 97], "191": [1, 71, 4, 74, 10, 72, 19, 241, 31, 235, 46, 202], "192": [1, 71, 1, 1, 4, 74, 10, 75, 19, 241, 31, 80, 46, 76], "193": [1, 33, 1, 193, 7, 98, 13, 104, 19, 49, 25, 197, 31, 48, 37, 129, 43, 103], "194": [1, 55, 1, 39, 11, 21, 21, 133, 31, 89, 41, 240, 51, 54, 51, 114], "195": [1, 55, 1, 39, 11, 21, 23, 133, 35, 89, 47, 240, 59, 54, 59, 114], "196": [1, 33, 1, 39, 8, 28, 16, 93, 23, 98, 30, 129, 36, 60, 42, 244, 47, 94, 52, 234], "197": [1, 33, 1, 39, 8, 28, 16, 228, 23, 98, 30, 109, 36, 185, 42, 212, 47, 103, 52, 236], "198": [1, 64, 11, 228, 16, 114, 26, 101, 31, 185, 41, 212], "199": [1, 174, 1, 33, 6, 45, 15, 55, 20, 93, 29, 50, 34, 29, 43, 207, 48, 94], "200": [1, 45, 1, 149, 6, 180, 12, 109, 19, 212, 27, 60, 36, 220, 46, 195], "201": [1, 237], "202": [1, 68, 1, 243, 1, 219, 1, 194], "203": [1, 33, 1, 45, 1, 93, 1, 23, 7, 93, 13, 23, 20, 97, 30, 226, 41, 60, 54, 242], "204": [1, 33, 1, 182, 8, 120, 15, 36, 22, 229, 29, 117, 36, 153, 43, 191, 50, 38], "205": [1, 33, 1, 182, 1, 120, 8, 120, 15, 36, 22, 229, 29, 117, 39, 153, 49, 191, 59, 38], "206": [1, 99, 5, 111, 13, 137, 18, 180, 26, 228, 30, 103, 38, 36], "207": [1, 40, 6, 28, 13, 106, 20, 98, 28, 185, 36, 163, 44, 103, 52, 12], "208": [1, 33, 1, 103, 10, 20, 14, 88, 23, 106, 27, 99, 36, 201, 40, 21, 49, 242], "209": [1, 33, 1, 184, 4, 39, 8, 204, 13, 44, 19, 122, 26, 46, 34, 99, 43, 36], "210": [1, 33, 1, 184, 4, 39, 8, 204, 13, 44, 19, 122, 28, 46, 38, 99, 51, 36], "211": [1, 33, 1, 40, 10, 106, 10, 107, 19, 55, 28, 42, 37, 36, 46, 56], "212": [1, 98, 1, 43, 6, 116, 12, 228, 18, 206, 24, 97, 30, 232, 36, 163, 42, 14, 48, 104], "213": [1, 132, 1, 110, 9, 35, 14, 227, 23, 219, 28, 117, 37, 156], "214": [1, 33, 1, 43, 6, 30, 12, 203, 19, 31, 27, 68, 35, 36, 44, 179, 54, 224], "215": [1, 10, 1, 43, 9, 98, 17, 103, 25, 185, 33, 154, 41, 97, 49, 163, 57, 251], "216": [1, 10, 1, 43, 8, 122, 15, 154, 22, 185, 29, 156, 36, 163, 43, 173, 50, 37], "217": [1, 10, 1, 43, 1, 122, 1, 154, 8, 122, 15, 154, 22, 185, 29, 156, 39, 163, 49, 173, 59, 37], "218": [1, 123, 8, 52, 15, 88, 22, 106, 29, 133, 36, 53, 43, 157, 50, 34], "219": [1, 123, 1, 52, 1, 88, 8, 52, 15, 88, 22, 106, 29, 133, 36, 53, 48, 157, 60, 34], "220": [1, 33, 10, 181, 19, 203, 28, 36, 37, 54, 46, 59], "221": [1, 30, 1, 181, 1, 203, 10, 181, 19, 203, 28, 36, 33, 31, 42, 54, 56, 59], "222": [1, 33, 7, 106, 13, 145, 19, 105, 25, 61, 31, 131, 37, 243, 43, 246], "223": [1, 55, 11, 199, 22, 60, 22, 62, 22, 61, 33, 116, 44, 58, 55, 63], "224": [1, 55, 11, 132, 22, 60, 22, 62, 22, 61, 25, 190, 38, 116, 54, 58, 70, 63], "225": [1, 217], "226": [1, 33, 1, 145, 10, 48, 18, 61, 25, 36, 32, 97, 40, 17, 49, 109], "227": [1, 43, 1, 64, 13, 28, 19, 129, 25, 97, 37, 31, 49, 211], "228": [1, 43, 1, 52, 7, 46, 13, 123, 20, 44, 27, 185, 35, 53, 43, 242], "229": [1, 43, 1, 52, 7, 46, 13, 123, 20, 44, 30, 185, 41, 53, 52, 242], "230": [1, 145, 1, 108, 1, 43, 1, 55, 8, 108, 15, 43, 22, 55, 29, 239, 40, 97, 51, 56], "231": [1, 33, 1, 45, 9, 111, 17, 175, 25, 36, 33, 205, 41, 203, 49, 38], "232": [1, 30, 1, 45, 9, 111, 17, 175, 25, 31, 33, 205, 41, 229, 49, 89], "233": [1, 176, 1, 33, 1, 160, 9, 97, 12, 60, 20, 105, 24, 111, 32, 199, 36, 161, 44, 192], "234": [1, 33, 8, 43, 15, 95, 23, 23, 31, 28, 40, 36, 49, 109], "235": [1, 166, 11, 166, 21, 166, 31, 166, 41, 166, 51, 166, 61, 166, 71, 166, 81, 166, 91, 166], "236": [1, 33], "237": [1, 27, 7, 116, 13, 228, 19, 98, 25, 229, 31, 68, 37, 97, 43, 197, 49, 167], "238": [1, 1, 1, 122, 9, 186, 13, 181, 21, 93, 25, 47, 33, 212, 37, 94, 45, 195, 49, 59], "239": [1, 98, 1, 43, 9, 9, 17, 113, 25, 129, 33, 103, 41, 85, 49, 87], "240": [1, 52, 7, 43, 13, 123, 19, 7, 25, 108, 31, 241, 37, 53, 43, 109, 49, 126], "241": [1, 33, 4, 45, 8, 111, 13, 23, 19, 208, 26, 117, 34, 205, 43, 34, 53, 215], "242": [1, 1, 4, 45, 7, 39, 10, 135, 13, 3, 18, 107, 23, 47, 28, 121, 33, 111, 40, 113, 47, 38], "243": [1, 44, 1, 43, 11, 84, 21, 46, 31, 98, 41, 209, 51, 115, 61, 242, 71, 87], "244": [1, 44, 1, 43, 11, 52, 21, 46, 31, 83, 41, 23, 51, 53, 61, 207, 71, 126], "245": [1, 44, 1, 43, 11, 55, 21, 46, 31, 16, 41, 61, 51, 54, 61, 243, 71, 56], "246": [1, 44, 1, 43, 8, 201, 15, 103, 22, 157, 29, 37, 36, 184, 43, 242, 50, 89, 57, 63], "247": [1, 44, 1, 43, 1, 201, 1, 103, 8, 201, 15, 103, 22, 157, 29, 37, 38, 184, 47, 242, 56, 89, 65, 63], "248": [1, 44, 1, 43, 1, 201, 1, 103, 8, 201, 15, 103, 22, 157, 29, 37, 38, 184, 47, 242, 61, 89, 75, 63], "249": [1, 177, 11, 219, 22, 16, 33, 105, 44, 56, 55, 240, 66, 129, 77, 18, 88, 246, 99, 248], "250": [1, 221, 11, 219, 22, 16, 33, 105, 44, 126, 55, 241, 66, 129, 77, 18, 88, 246, 99, 248], "251": [1, 73, 1, 93, 1, 105, 1, 215, 10, 219, 20, 246, 30, 248, 40, 226, 50, 195]};

    const GEN2_ITEMS = [{"name": "No Item", "id": 0}, {"name": "Amulet Coin", "id": 91}, {"name": "Antidote", "id": 9}, {"name": "Awakening", "id": 12}, {"name": "Basement Key", "id": 133}, {"name": "Berry", "id": 173}, {"name": "Berry Juice", "id": 139}, {"name": "Berserk Gene", "id": 152}, {"name": "Bicycle", "id": 7}, {"name": "Big Mushroom", "id": 87}, {"name": "Big Pearl", "id": 111}, {"name": "Bitter Berry", "id": 83}, {"name": "Blackbelt", "id": 98}, {"name": "Black Glasses", "id": 102}, {"name": "Blk Apricorn", "id": 99}, {"name": "Blu Apricorn", "id": 89}, {"name": "Blue Card (C Only)", "id": 116}, {"name": "Bluesky Mail", "id": 187}, {"name": "Brick Piece", "id": 180}, {"name": "Bright Powder", "id": 3}, {"name": "Burn Heal", "id": 10}, {"name": "Burnt Berry", "id": 79}, {"name": "Calcium", "id": 31}, {"name": "Carbos", "id": 29}, {"name": "Card Key", "id": 127}, {"name": "Charcoal", "id": 138}, {"name": "Cleanse Tag", "id": 94}, {"name": "Clear Bell (C Only)", "id": 70}, {"name": "Coin Case", "id": 54}, {"name": "Dire Hit", "id": 44}, {"name": "Dragon Fang", "id": 144}, {"name": "Dragon Scale", "id": 151}, {"name": "Egg Ticket (C Only)", "id": 129}, {"name": "Elixir", "id": 65}, {"name": "Energy Powder", "id": 121}, {"name": "Energy Root", "id": 122}, {"name": "Eon Mail", "id": 185}, {"name": "Escape Rope", "id": 19}, {"name": "Ether", "id": 63}, {"name": "Everstone", "id": 112}, {"name": "Exp. Share", "id": 57}, {"name": "Fast Ball", "id": 161}, {"name": "Fire Stone", "id": 22}, {"name": "Flower Mail", "id": 158}, {"name": "Focus Band", "id": 119}, {"name": "Fresh Water", "id": 46}, {"name": "Friend Ball", "id": 164}, {"name": "Full Heal", "id": 38}, {"name": "Full Restore", "id": 14}, {"name": "Gold Berry", "id": 174}, {"name": "Gold Leaf", "id": 75}, {"name": "Good Rod", "id": 59}, {"name": "Gorgeous Box", "id": 168}, {"name": "Great Ball", "id": 4}, {"name": "Grn Apricorn", "id": 93}, {"name": "GS Ball (C Only)", "id": 115}, {"name": "Guard Spec.", "id": 41}, {"name": "Hard Stone", "id": 125}, {"name": "Heal Powder", "id": 123}, {"name": "Heavy Ball", "id": 157}, {"name": "HM01", "id": 243}, {"name": "HM02", "id": 244}, {"name": "HM03", "id": 245}, {"name": "HM04", "id": 246}, {"name": "HM05", "id": 247}, {"name": "HM06", "id": 248}, {"name": "HM07", "id": 249}, {"name": "HP Up", "id": 26}, {"name": "Hyper Potion", "id": 16}, {"name": "Ice Berry", "id": 80}, {"name": "Ice Heal", "id": 11}, {"name": "Iron", "id": 28}, {"name": "Itemfinder", "id": 55}, {"name": "King's Rock", "id": 82}, {"name": "Leaf Stone", "id": 34}, {"name": "Leftovers", "id": 146}, {"name": "Lemonade", "id": 48}, {"name": "Level Ball", "id": 159}, {"name": "Light Ball", "id": 163}, {"name": "Litebluemail", "id": 182}, {"name": "Lost Item", "id": 130}, {"name": "Love Ball", "id": 166}, {"name": "Lovely Mail", "id": 184}, {"name": "Lucky Egg", "id": 126}, {"name": "Lucky Punch", "id": 30}, {"name": "Lure Ball", "id": 160}, {"name": "Machine Part", "id": 128}, {"name": "Magnet", "id": 108}, {"name": "Master Ball", "id": 1}, {"name": "Max Elixir", "id": 21}, {"name": "Max Ether", "id": 64}, {"name": "Max Potion", "id": 15}, {"name": "Max Repel", "id": 43}, {"name": "Max Revive", "id": 40}, {"name": "Metal Coat", "id": 143}, {"name": "Metal Powder", "id": 35}, {"name": "Mint Berry", "id": 84}, {"name": "MiracleBerry", "id": 109}, {"name": "Miracle Seed", "id": 117}, {"name": "Mirage Mail", "id": 189}, {"name": "Moomoo Milk", "id": 72}, {"name": "Moon Ball", "id": 165}, {"name": "Moon Stone", "id": 8}, {"name": "Morph Mail", "id": 186}, {"name": "Music Mail", "id": 188}, {"name": "Mystery Berry", "id": 150}, {"name": "Mystery Egg", "id": 69}, {"name": "Mystic Water", "id": 95}, {"name": "Never-Melt Ice", "id": 107}, {"name": "Normal Box", "id": 167}, {"name": "Nugget", "id": 36}, {"name": "Old Rod", "id": 58}, {"name": "Paralyze Heal", "id": 13}, {"name": "Park Ball", "id": 177}, {"name": "Pass", "id": 134}, {"name": "Pearl", "id": 110}, {"name": "Pink Bow", "id": 104}, {"name": "Pnk Apricorn", "id": 101}, {"name": "Poison Barb", "id": 81}, {"name": "Poke Ball", "id": 5}, {"name": "Poke Doll", "id": 37}, {"name": "Polkadot Bow", "id": 170}, {"name": "Portraitmail", "id": 183}, {"name": "Potion", "id": 18}, {"name": "PP Up", "id": 62}, {"name": "Protein", "id": 27}, {"name": "PRZCureBerry", "id": 78}, {"name": "PSNCureBerry", "id": 74}, {"name": "Quick Claw", "id": 73}, {"name": "Rage Candy Bar", "id": 114}, {"name": "Rainbow Wing", "id": 178}, {"name": "Rare Candy", "id": 32}, {"name": "Red Apricorn", "id": 85}, {"name": "Red Scale", "id": 66}, {"name": "Repel", "id": 20}, {"name": "Revival Herb", "id": 124}, {"name": "Revive", "id": 39}, {"name": "Sacred Ash", "id": 156}, {"name": "Scope Lens", "id": 140}, {"name": "Secret Potion", "id": 67}, {"name": "Sharp Beak", "id": 77}, {"name": "Silver Leaf", "id": 60}, {"name": "Silver Powder", "id": 88}, {"name": "Silver Wing", "id": 71}, {"name": "Slowpoke Tail", "id": 103}, {"name": "Smoke Ball", "id": 106}, {"name": "Soda Pop", "id": 47}, {"name": "Soft Sand", "id": 76}, {"name": "Spell Tag", "id": 113}, {"name": "Squirt Bottle", "id": 175}, {"name": "S.S. Ticket", "id": 68}, {"name": "Stardust", "id": 131}, {"name": "Star Piece", "id": 132}, {"name": "Stick", "id": 105}, {"name": "Sun Stone", "id": 169}, {"name": "Super Potion", "id": 17}, {"name": "Super Repel", "id": 42}, {"name": "Super Rod", "id": 61}, {"name": "Surf Mail", "id": 181}, {"name": "Teru-sama", "id": 90}, {"name": "Teru-sama", "id": 120}, {"name": "Thick Club", "id": 118}, {"name": "Thunder Stone", "id": 23}, {"name": "Tiny Mushroom", "id": 86}, {"name": "TM01", "id": 191}, {"name": "TM02", "id": 192}, {"name": "TM03", "id": 193}, {"name": "TM04", "id": 194}, {"name": "TM05", "id": 196}, {"name": "TM06", "id": 197}, {"name": "TM07", "id": 198}, {"name": "TM08", "id": 199}, {"name": "TM09", "id": 200}, {"name": "TM10", "id": 201}, {"name": "TM11", "id": 202}, {"name": "TM12", "id": 203}, {"name": "TM13", "id": 204}, {"name": "TM14", "id": 205}, {"name": "TM15", "id": 206}, {"name": "TM16", "id": 207}, {"name": "TM17", "id": 208}, {"name": "TM18", "id": 209}, {"name": "TM19", "id": 210}, {"name": "TM20", "id": 211}, {"name": "TM21", "id": 212}, {"name": "TM22", "id": 213}, {"name": "TM23", "id": 214}, {"name": "TM24", "id": 215}, {"name": "TM25", "id": 216}, {"name": "TM26", "id": 217}, {"name": "TM27", "id": 218}, {"name": "TM28", "id": 219}, {"name": "TM29", "id": 221}, {"name": "TM30", "id": 222}, {"name": "TM31", "id": 223}, {"name": "TM32", "id": 224}, {"name": "TM33", "id": 225}, {"name": "TM34", "id": 226}, {"name": "TM35", "id": 227}, {"name": "TM36", "id": 228}, {"name": "TM37", "id": 229}, {"name": "TM38", "id": 230}, {"name": "TM39", "id": 231}, {"name": "TM40", "id": 232}, {"name": "TM41", "id": 233}, {"name": "TM42", "id": 234}, {"name": "TM43", "id": 235}, {"name": "TM44", "id": 236}, {"name": "TM45", "id": 237}, {"name": "TM46", "id": 238}, {"name": "TM47", "id": 239}, {"name": "TM48", "id": 240}, {"name": "TM49", "id": 241}, {"name": "TM50", "id": 242}, {"name": "Twisted Spoon", "id": 96}, {"name": "Ultra Ball", "id": 2}, {"name": "Up-Grade", "id": 172}, {"name": "Water Stone", "id": 24}, {"name": "Wht Apricorn", "id": 97}, {"name": "X Accuracy", "id": 33}, {"name": "X Attack", "id": 49}, {"name": "X Defend", "id": 51}, {"name": "X Special", "id": 53}, {"name": "X Speed", "id": 52}, {"name": "Ylw Apricorn", "id": 92}];

    function populateSpeciesSelect() {
      const sel = document.getElementById("species");
      if (!sel) return;
      const prevVal = parseInt(sel.value) || 94;
      const maxSp = currentGen === 1 ? 151 : 251;
      sel.innerHTML = "";
      for (let i = 1; i <= maxSp; i++) {
        const opt = document.createElement("option");
        opt.value = i;
        opt.textContent = "#" + String(i).padStart(3, "0") + " - " + POKEMON_NAMES[i - 1];
        if (i === prevVal) opt.selected = true;
        sel.appendChild(opt);
      }
      if (prevVal > maxSp) {
        sel.value = 1;
        onSpeciesChange();
      }
    }

    function populateItemsSelect() {
      const sel = document.getElementById("item");
      if (!sel) return;
      sel.innerHTML = "";
      GEN2_ITEMS.forEach(it => {
        const opt = document.createElement("option");
        opt.value = it.id;
        opt.textContent = it.name;
        sel.appendChild(opt);
      });
    }

    function getDefaultMoves(speciesId, level, gen) {
      const data = POKEMON_LEARNSETS[speciesId];
      if (!data || data.length === 0) return [0, 0, 0, 0];

      const moves = [];
      for (let i = 0; i < data.length; i += 2) {
        const lvl = data[i];
        const mId = data[i + 1];
        if (lvl <= level) {
          if (gen === 1) {
            const mObj = ALL_MOVES.find(m => m.id === mId);
            if (mObj && mObj.gen !== 1) continue;
          }
          moves.push(mId);
          if (moves.length > 4) {
            moves.shift();
          }
        }
      }

      while (moves.length < 4) {
        moves.push(0);
      }
      return moves;
    }

    function applyDefaultMoves(speciesId, level) {
      const sp = speciesId || (parseInt(document.getElementById('species').value) || 1);
      const lvl = level || (parseInt(document.getElementById('level').value) || 50);
      const defMoves = getDefaultMoves(sp, lvl, currentGen);
      ['m0', 'm1', 'm2', 'm3'].forEach((id, idx) => {
        const el = document.getElementById(id);
        if (el) el.value = defMoves[idx] || 0;
      });
    }

    function onLevelChange() {
      const sp = parseInt(document.getElementById("species").value) || 1;
      const lvl = parseInt(document.getElementById("level").value) || 50;
      applyDefaultMoves(sp, lvl);
      updatePreview();
    }

    function onSpeciesChange() {
      const sp = parseInt(document.getElementById("species").value) || 1;
      const lvl = parseInt(document.getElementById("level").value) || 50;
      const name = POKEMON_NAMES[sp - 1] || "POKEMON";
      document.getElementById("nickname").value = name.toUpperCase().slice(0, 10);
      applyDefaultMoves(sp, lvl);
      updatePreview();
    }

    let ws;

    function showToast(msg) {
      const t = document.getElementById('toast');
      t.textContent = msg;
      t.classList.add('show');
      setTimeout(() => t.classList.remove('show'), 2500);
    }

    function initWS() {
      const proto = location.protocol === 'https:' ? 'wss:' : 'ws:';
      ws = new WebSocket(`${proto}//${location.host}/ws`);
      ws.onmessage = (event) => {
        try {
          const data = JSON.parse(event.data);
          handleWSMessage(data);
        } catch(e) {}
      };
      ws.onclose = () => setTimeout(initWS, 2000);
    }

    function log(msg) {
      const d = new Date();
      const time = d.toTimeString().split(' ')[0];
      const box = document.getElementById('consoleLog');
      box.innerHTML += `[${time}] ${msg}<br>`;
      box.scrollTop = box.scrollHeight;
    }

    function handleWSMessage(data) {
      if (data.type === 'status') {
        const card = document.getElementById('statusCard');
        const dot = document.getElementById('statusDot');
        const title = document.getElementById('statusTitle');
        const desc = document.getElementById('statusDesc');

        title.textContent = data.title;
        desc.textContent = data.desc;

        if (data.connected) {
          card.classList.add('connected');
          dot.style.background = 'var(--status-green)';
        } else {
          card.classList.remove('connected');
          dot.style.background = 'var(--status-red)';
        }
        log(`${data.title}: ${data.desc}`);

        if (data.gen && parseInt(data.gen) !== currentGen) {
          const genSel = document.getElementById('genSelect');
          if (genSel) {
            genSel.value = data.gen;
            changeGeneration(false);
          }
        }
      } else if (data.type === 'generation') {
        const gen = parseInt(data.gen);
        const genSel = document.getElementById('genSelect');
        if (genSel && parseInt(genSel.value) !== gen) {
          genSel.value = gen;
          changeGeneration(false);
          log(`Generación detectada automáticamente: Gen ${gen === 1 ? 'I (Kanto)' : 'II (Johto)'}`);
          showToast(`Auto-detectado: Generación ${gen === 1 ? 'I' : 'II'}`);
        }
      } else if (data.type === 'received_pokemon') {
        document.getElementById('noReceivedNotice').style.display = 'none';
        document.getElementById('receivedData').style.display = 'block';

        const recSp = String(data.species).padStart(3, '0');
        document.getElementById('recSpriteImg').src = '/sprites/' + recSp + '.png';
        document.getElementById('recSummaryName').textContent = (data.nickname || data.name).toUpperCase();
        document.getElementById('recSummaryDex').textContent = '#' + recSp;
        document.getElementById('recSummaryDetails').textContent = 'Nivel ' + data.level + ' • ' + data.name + (data.shiny ? ' ✨ Shiny' : '');

        document.getElementById('rec_species').value = data.name + ' (ID ' + data.species + ')';
        document.getElementById('rec_level').value = data.level;
        document.getElementById('rec_nick').value = data.nickname;
        document.getElementById('rec_m0').value = data.moves[0] || 0;
        document.getElementById('rec_m1').value = data.moves[1] || 0;
        document.getElementById('rec_m2').value = data.moves[2] || 0;
        document.getElementById('rec_m3').value = data.moves[3] || 0;
        document.getElementById('rec_shiny').checked = !!data.shiny;

        // Auto-switch to Received tab
        switchTab('tab-receive', document.querySelectorAll('.tab-btn')[1]);

        log(`📥 Recibido de Game Boy: ${data.name} (Lvl ${data.level})${data.shiny ? ' ✨ Shiny' : ''}`);
        showToast(`¡${data.name} recibido de Game Boy listo para editar o devolver!`);
      }
    }

    function switchTab(tabId, btn) {
      document.querySelectorAll('.tab-pane').forEach(el => el.classList.remove('active'));
      document.querySelectorAll('.tab-btn').forEach(el => el.classList.remove('active'));
      document.getElementById(tabId).classList.add('active');
      btn.classList.add('active');
    }

    function updatePreview() {
      const sp = parseInt(document.getElementById('species').value) || 1;
      const nick = document.getElementById('nickname').value || 'POKÉMON';
      const lvl = document.getElementById('level').value || 50;
      const shiny = document.getElementById('shiny') ? document.getElementById('shiny').checked : false;

      document.getElementById('previewName').textContent = nick.toUpperCase();
      document.getElementById('previewDex').textContent = '#' + String(sp).padStart(3, '0');
      document.getElementById('previewDetails').textContent = `Nivel ${lvl} • ${shiny ? '✨ Shiny • ' : ''}ID ${sp}`;

      const spriteBox = document.querySelector('.pkmn-sprite');
      if (spriteBox) {
        if (shiny) {
          spriteBox.classList.add('shiny');
        } else {
          spriteBox.classList.remove('shiny');
        }
      }

      // Sprite servido directamente por el ESP32 desde la flash PROGMEM
      const spriteNum = String(sp).padStart(3, '0');
      document.getElementById('spriteImg').src = '/sprites/' + spriteNum + '.png';
    }

    function changeGeneration(notifyBackend = true) {
      currentGen = parseInt(document.getElementById('genSelect').value);
      document.getElementById('genBadge').textContent = currentGen === 1 ? 'GEN I' : 'GEN II';
      document.getElementById('gen2Options').style.display = currentGen === 2 ? 'block' : 'none';
      if (currentGen === 1) {
        document.getElementById('shiny').checked = false;
        document.getElementById('item').value = 0;
      }
      populateSpeciesSelect();
      populateMovesSelect(currentGen);
      if (notifyBackend) {
        fetch('/api/generation', {
          method: 'POST',
          headers: {'Content-Type': 'application/json'},
          body: JSON.stringify({ gen: currentGen })
        });
      }
      log(`Generación cambiada a: Gen ${currentGen === 1 ? 'I (Kanto)' : 'II (Johto)'}`);
      showToast(`Cambiado a Generación ${currentGen === 1 ? 'I' : 'II'}`);
      updatePreview();
    }

    function applyPreset(preset) {
      if (preset === 'gengar') {
        document.getElementById('species').value = 94;
        document.getElementById('nickname').value = 'GENGAR';
        document.getElementById('level').value = 50;
        document.getElementById('m0').value = 138;
        document.getElementById('m1').value = 85;
        document.getElementById('m2').value = 94;
        document.getElementById('m3').value = 69;
      } else if (preset === 'machamp') {
        document.getElementById('species').value = 68;
        document.getElementById('nickname').value = 'MACHAMP';
        document.getElementById('level').value = 50;
        document.getElementById('m0').value = 22;
        document.getElementById('m1').value = 70;
        document.getElementById('m2').value = 90;
        document.getElementById('m3').value = 25;
      } else if (preset === 'alakazam') {
        document.getElementById('species').value = 65;
        document.getElementById('nickname').value = 'ALAKAZAM';
        document.getElementById('level').value = 50;
        document.getElementById('m0').value = 94;
        document.getElementById('m1').value = 105;
        document.getElementById('m2').value = 115;
        document.getElementById('m3').value = 129;
      } else if (preset === 'golem') {
        document.getElementById('species').value = 76;
        document.getElementById('nickname').value = 'GOLEM';
        document.getElementById('level').value = 50;
        document.getElementById('m0').value = 89;
        document.getElementById('m1').value = 88;
        document.getElementById('m2').value = 153;
        document.getElementById('m3').value = 23;
      } else if (preset === 'steelix') {
        document.getElementById('genSelect').value = 2;
        changeGeneration();
        document.getElementById('species').value = 208;
        document.getElementById('nickname').value = 'STEELIX';
        document.getElementById('level').value = 50;
        document.getElementById('m0').value = 231;
        document.getElementById('m1').value = 89;
        document.getElementById('m2').value = 157;
        document.getElementById('m3').value = 23;
      } else if (preset === 'scizor') {
        document.getElementById('genSelect').value = 2;
        changeGeneration();
        document.getElementById('species').value = 212;
        document.getElementById('nickname').value = 'SCIZOR';
        document.getElementById('level').value = 50;
        document.getElementById('m0').value = 232;
        document.getElementById('m1').value = 14;
        document.getElementById('m2').value = 97;
        document.getElementById('m3').value = 210;
      } else if (preset === 'kingdra') {
        document.getElementById('genSelect').value = 2;
        changeGeneration();
        document.getElementById('species').value = 230;
        document.getElementById('nickname').value = 'KINGDRA';
        document.getElementById('level').value = 50;
        document.getElementById('m0').value = 56;
        document.getElementById('m1').value = 225;
        document.getElementById('m2').value = 82;
        document.getElementById('m3').value = 61;
      } else if (preset === 'mew') {
        document.getElementById('species').value = 151;
        document.getElementById('nickname').value = 'MEW';
        document.getElementById('ot_name').value = 'MYSTRY';
        document.getElementById('ot_id').value = 69;
        document.getElementById('level').value = 30;
        document.getElementById('m0').value = 1;
        document.getElementById('m1').value = 104;
        document.getElementById('m2').value = 129;
        document.getElementById('m3').value = 94;
      } else if (preset === 'celebi') {
        document.getElementById('genSelect').value = 2;
        changeGeneration();
        document.getElementById('species').value = 251;
        document.getElementById('nickname').value = 'CELEBI';
        document.getElementById('ot_name').value = 'WIN2011';
        document.getElementById('ot_id').value = 1121;
        document.getElementById('level').value = 50;
        document.getElementById('m0').value = 73;
        document.getElementById('m1').value = 105;
        document.getElementById('m2').value = 113;
        document.getElementById('m3').value = 247;
      }
      updatePreview();
      log(`Preset aplicado: ${preset}`);
      showToast(`Preset ${preset.toUpperCase()} aplicado`);
    }

    function savePokemon(e) {
      e.preventDefault();
      const payload = {
        species: parseInt(document.getElementById('species').value) - 1,
        level: parseInt(document.getElementById('level').value),
        nickname: document.getElementById('nickname').value,
        ot_name: document.getElementById('ot_name').value,
        ot_id: parseInt(document.getElementById('ot_id').value),
        m0: parseInt(document.getElementById('m0').value) || 0,
        m1: parseInt(document.getElementById('m1').value) || 0,
        m2: parseInt(document.getElementById('m2').value) || 0,
        m3: parseInt(document.getElementById('m3').value) || 0,
        eviv: parseInt(document.getElementById('eviv').value),
        shiny: document.getElementById('shiny').checked,
        item: parseInt(document.getElementById('item').value) || 0
      };

      fetch('/api/configure', {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify(payload)
      }).then(() => {
        showToast('¡Cargado en la mesa de trade!');
        log(`Pokémon configurado: ${payload.nickname} (Nivel ${payload.level})`);
      });
    }

    function returnModifiedPokemon() {
      const payload = {
        level: parseInt(document.getElementById('rec_level').value),
        nickname: document.getElementById('rec_nick').value,
        m0: parseInt(document.getElementById('rec_m0').value) || 0,
        m1: parseInt(document.getElementById('rec_m1').value) || 0,
        m2: parseInt(document.getElementById('rec_m2').value) || 0,
        m3: parseInt(document.getElementById('rec_m3').value) || 0,
        shiny: document.getElementById('rec_shiny').checked,
        item: 0
      };

      fetch('/api/return_traded', {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify(payload)
      }).then(() => {
        showToast('¡Preparado para devolver a Game Boy!');
        log(`Devolución preparada: ${payload.nickname}`);
      });
    }

    window.onload = () => {
      populateSpeciesSelect();
      populateItemsSelect();
      populateMovesSelect(currentGen);
      applyPreset('gengar');
      initWS();
      updatePreview();
    };
  </script>
</body>
</html>
)rawliteral";
