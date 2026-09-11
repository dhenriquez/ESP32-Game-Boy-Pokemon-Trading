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
      <div class="brand-subtitle">ESP32-S3 Game Boy Link Station - @dhenriquez</div>
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
              <input type="number" id="level" min="1" max="100" value="50" required onchange="updatePreview()">
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

          <div class="grid-2">
            <div class="form-group">
              <label>Ataque 1</label>
              <input type="text" id="m0" list="moves_list" value="Dream Eater" autocomplete="off" placeholder="Buscar ataque...">
            </div>
            <div class="form-group">
              <label>Ataque 2</label>
              <input type="text" id="m1" list="moves_list" value="Thunderbolt" autocomplete="off" placeholder="Buscar ataque...">
            </div>
          </div>
          <div class="grid-2">
            <div class="form-group">
              <label>Ataque 3</label>
              <input type="text" id="m2" list="moves_list" value="Psychic" autocomplete="off" placeholder="Buscar ataque...">
            </div>
            <div class="form-group">
              <label>Ataque 4</label>
              <input type="text" id="m3" list="moves_list" value="Seismic Toss" autocomplete="off" placeholder="Buscar ataque...">
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
              <input type="text" id="rec_m0" list="moves_list" autocomplete="off" placeholder="Buscar ataque...">
            </div>
            <div class="form-group">
              <label>Ataque 2</label>
              <input type="text" id="rec_m1" list="moves_list" autocomplete="off" placeholder="Buscar ataque...">
            </div>
          </div>
          <div class="grid-2">
            <div class="form-group">
              <label>Ataque 3</label>
              <input type="text" id="rec_m2" list="moves_list" autocomplete="off" placeholder="Buscar ataque...">
            </div>
            <div class="form-group">
              <label>Ataque 4</label>
              <input type="text" id="rec_m3" list="moves_list" autocomplete="off" placeholder="Buscar ataque...">
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

  <datalist id="moves_list"></datalist>
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

    function populateMovesDatalist(gen) {
      const dl = document.getElementById("moves_list");
      if (!dl) return;
      dl.innerHTML = "";
      ALL_MOVES.forEach(m => {
        if (gen === 1 && m.gen !== 1) return;
        const opt = document.createElement("option");
        opt.value = m.name;
        dl.appendChild(opt);
      });
    }

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

    function onSpeciesChange() {
      const sp = parseInt(document.getElementById("species").value) || 1;
      const name = POKEMON_NAMES[sp - 1] || "POKEMON";
      document.getElementById("nickname").value = name.toUpperCase().slice(0, 10);
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
      } else if (data.type === 'received_pokemon') {
        document.getElementById('noReceivedNotice').style.display = 'none';
        document.getElementById('receivedData').style.display = 'block';

        const recSp = String(data.species).padStart(3, '0');
        document.getElementById('recSpriteImg').src = '/sprites/' + recSp + '.png';
        document.getElementById('recSummaryName').textContent = (data.nickname || data.name).toUpperCase();
        document.getElementById('recSummaryDex').textContent = '#' + recSp;
        document.getElementById('recSummaryDetails').textContent = 'Nivel ' + data.level + ' • ' + data.name;

        document.getElementById('rec_species').value = data.name + ' (ID ' + data.species + ')';
        document.getElementById('rec_level').value = data.level;
        document.getElementById('rec_nick').value = data.nickname;
        document.getElementById('rec_m0').value = moveIdToName(data.moves[0]);
        document.getElementById('rec_m1').value = moveIdToName(data.moves[1]);
        document.getElementById('rec_m2').value = moveIdToName(data.moves[2]);
        document.getElementById('rec_m3').value = moveIdToName(data.moves[3]);
        log(`Recibido de Game Boy: ${data.name} (Lvl ${data.level})`);
        showToast(`¡${data.name} recibido de Game Boy!`);
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

    function changeGeneration() {
      currentGen = parseInt(document.getElementById('genSelect').value);
      document.getElementById('genBadge').textContent = currentGen === 1 ? 'GEN I' : 'GEN II';
      document.getElementById('gen2Options').style.display = currentGen === 2 ? 'block' : 'none';
      if (currentGen === 1) {
        document.getElementById('shiny').checked = false;
        document.getElementById('item').value = 0;
        ['m0', 'm1', 'm2', 'm3'].forEach(id => {
          const el = document.getElementById(id);
          if (el) {
            const mId = moveToId(el.value);
            const mObj = ALL_MOVES.find(m => m.id === mId);
            if (mObj && mObj.gen === 2) {
              el.value = "No Move";
            }
          }
        });
      }
      populateSpeciesSelect();
      populateMovesDatalist(currentGen);
      fetch('/api/generation', {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify({ gen: currentGen })
      });
      log(`Cambiado a Generación ${currentGen === 1 ? 'I' : 'II'}`);
      showToast(`Cambiado a Generación ${currentGen === 1 ? 'I' : 'II'}`);
      updatePreview();
    }

    function applyPreset(preset) {
      if (preset === 'gengar') {
        document.getElementById('species').value = 94;
        document.getElementById('nickname').value = 'GENGAR';
        document.getElementById('level').value = 50;
        document.getElementById('m0').value = moveIdToName(138);
        document.getElementById('m1').value = moveIdToName(85);
        document.getElementById('m2').value = moveIdToName(94);
        document.getElementById('m3').value = moveIdToName(69);
      } else if (preset === 'machamp') {
        document.getElementById('species').value = 68;
        document.getElementById('nickname').value = 'MACHAMP';
        document.getElementById('level').value = 50;
        document.getElementById('m0').value = moveIdToName(22);
        document.getElementById('m1').value = moveIdToName(70);
        document.getElementById('m2').value = moveIdToName(90);
        document.getElementById('m3').value = moveIdToName(25);
      } else if (preset === 'alakazam') {
        document.getElementById('species').value = 65;
        document.getElementById('nickname').value = 'ALAKAZAM';
        document.getElementById('level').value = 50;
        document.getElementById('m0').value = moveIdToName(94);
        document.getElementById('m1').value = moveIdToName(105);
        document.getElementById('m2').value = moveIdToName(115);
        document.getElementById('m3').value = moveIdToName(129);
      } else if (preset === 'golem') {
        document.getElementById('species').value = 76;
        document.getElementById('nickname').value = 'GOLEM';
        document.getElementById('level').value = 50;
        document.getElementById('m0').value = moveIdToName(89);
        document.getElementById('m1').value = moveIdToName(88);
        document.getElementById('m2').value = moveIdToName(153);
        document.getElementById('m3').value = moveIdToName(23);
      } else if (preset === 'steelix') {
        document.getElementById('genSelect').value = 2;
        changeGeneration();
        document.getElementById('species').value = 208;
        document.getElementById('nickname').value = 'STEELIX';
        document.getElementById('level').value = 50;
        document.getElementById('m0').value = moveIdToName(231);
        document.getElementById('m1').value = moveIdToName(89);
        document.getElementById('m2').value = moveIdToName(157);
        document.getElementById('m3').value = moveIdToName(23);
      } else if (preset === 'scizor') {
        document.getElementById('genSelect').value = 2;
        changeGeneration();
        document.getElementById('species').value = 212;
        document.getElementById('nickname').value = 'SCIZOR';
        document.getElementById('level').value = 50;
        document.getElementById('m0').value = moveIdToName(232);
        document.getElementById('m1').value = moveIdToName(14);
        document.getElementById('m2').value = moveIdToName(97);
        document.getElementById('m3').value = moveIdToName(210);
      } else if (preset === 'kingdra') {
        document.getElementById('genSelect').value = 2;
        changeGeneration();
        document.getElementById('species').value = 230;
        document.getElementById('nickname').value = 'KINGDRA';
        document.getElementById('level').value = 50;
        document.getElementById('m0').value = moveIdToName(56);
        document.getElementById('m1').value = moveIdToName(225);
        document.getElementById('m2').value = moveIdToName(82);
        document.getElementById('m3').value = moveIdToName(61);
      } else if (preset === 'mew') {
        document.getElementById('species').value = 151;
        document.getElementById('nickname').value = 'MEW';
        document.getElementById('ot_name').value = 'MYSTRY';
        document.getElementById('ot_id').value = 69;
        document.getElementById('level').value = 30;
        document.getElementById('m0').value = moveIdToName(1);
        document.getElementById('m1').value = moveIdToName(104);
        document.getElementById('m2').value = moveIdToName(129);
        document.getElementById('m3').value = moveIdToName(94);
      } else if (preset === 'celebi') {
        document.getElementById('genSelect').value = 2;
        changeGeneration();
        document.getElementById('species').value = 251;
        document.getElementById('nickname').value = 'CELEBI';
        document.getElementById('ot_name').value = 'WIN2011';
        document.getElementById('ot_id').value = 1121;
        document.getElementById('level').value = 50;
        document.getElementById('m0').value = moveIdToName(73);
        document.getElementById('m1').value = moveIdToName(105);
        document.getElementById('m2').value = moveIdToName(113);
        document.getElementById('m3').value = moveIdToName(247);
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
        m0: moveToId(document.getElementById('m0').value),
        m1: moveToId(document.getElementById('m1').value),
        m2: moveToId(document.getElementById('m2').value),
        m3: moveToId(document.getElementById('m3').value),
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
        m0: moveToId(document.getElementById('rec_m0').value),
        m1: moveToId(document.getElementById('rec_m1').value),
        m2: moveToId(document.getElementById('rec_m2').value),
        m3: moveToId(document.getElementById('rec_m3').value),
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
      populateMovesDatalist(currentGen);
      initWS();
      updatePreview();
    };
  </script>
</body>
</html>
)rawliteral";
