#pragma once

#include <pgmspace.h>

static const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 Game Boy Pokémon Trader</title>
  <style>
    :root {
      --bg: #121417;
      --card-bg: #1e2229;
      --card-border: #2f3642;
      --accent: #e53e3e;
      --accent-blue: #3182ce;
      --accent-gold: #d69e2e;
      --text: #f7fafc;
      --text-muted: #a0aec0;
      --gb-green: #8bac0f;
      --gb-dark: #0f380f;
      --badge-red: #e53e3e;
      --badge-green: #38a169;
      --badge-yellow: #d69e2e;
      --badge-blue: #3182ce;
    }
    * { box-sizing: border-box; margin: 0; padding: 0; font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif; }
    body { background-color: var(--bg); color: var(--text); padding: 12px; max-width: 680px; margin: auto; }
    header { text-align: center; margin-bottom: 16px; border-bottom: 2px solid var(--card-border); padding-bottom: 12px; }
    h1 { font-size: 1.4rem; color: #ffcb05; text-shadow: 2px 2px #2a75bb; letter-spacing: 1px; }
    .subtitle { font-size: 0.8rem; color: var(--text-muted); margin-top: 4px; }
    
    /* Status banner */
    .status-card { background: var(--card-bg); border: 1px solid var(--card-border); border-radius: 12px; padding: 12px; margin-bottom: 16px; display: flex; align-items: center; justify-content: space-between; }
    .status-left { display: flex; align-items: center; gap: 10px; }
    .status-dot { width: 14px; height: 14px; border-radius: 50%; background: var(--badge-red); animation: pulse 1.5s infinite; }
    @keyframes pulse { 0% { opacity: 0.4; } 50% { opacity: 1; } 100% { opacity: 0.4; } }
    .status-title { font-weight: bold; font-size: 0.95rem; }
    .status-desc { font-size: 0.75rem; color: var(--text-muted); }
    .gen-badge { background: #2b6cb0; font-size: 0.75rem; padding: 4px 10px; border-radius: 20px; font-weight: bold; }
    
    /* Nav tabs */
    .tabs { display: flex; gap: 6px; margin-bottom: 16px; overflow-x: auto; padding-bottom: 4px; }
    .tab-btn { flex: 1; min-width: 90px; background: var(--card-bg); border: 1px solid var(--card-border); color: var(--text-muted); padding: 10px 6px; border-radius: 8px; cursor: pointer; font-size: 0.8rem; font-weight: 600; text-align: center; }
    .tab-btn.active { background: #2d3748; color: #ffcb05; border-color: #ffcb05; }
    
    /* Content sections */
    .tab-pane { display: none; }
    .tab-pane.active { display: block; }
    
    .card { background: var(--card-bg); border: 1px solid var(--card-border); border-radius: 12px; padding: 16px; margin-bottom: 14px; }
    .card-title { font-size: 1rem; color: #ffcb05; margin-bottom: 12px; display: flex; align-items: center; gap: 6px; }
    
    /* Form controls */
    .grid-2 { display: grid; grid-template-columns: 1fr 1fr; gap: 10px; }
    .form-group { margin-bottom: 12px; }
    label { display: block; font-size: 0.75rem; color: var(--text-muted); margin-bottom: 4px; text-transform: uppercase; letter-spacing: 0.5px; }
    input, select { width: 100%; background: #121417; border: 1px solid var(--card-border); color: var(--text); border-radius: 6px; padding: 8px 10px; font-size: 0.9rem; }
    input:focus, select:focus { outline: none; border-color: #ffcb05; }
    
    .btn { display: block; width: 100%; padding: 12px; border-radius: 8px; border: none; font-size: 0.95rem; font-weight: bold; cursor: pointer; text-align: center; transition: 0.2s; }
    .btn-primary { background: #e53e3e; color: white; }
    .btn-primary:hover { background: #c53030; }
    .btn-success { background: #38a169; color: white; }
    .btn-success:hover { background: #2f855a; }
    
    /* Presets pills */
    .presets { display: flex; flex-wrap: wrap; gap: 6px; margin-bottom: 14px; }
    .preset-pill { background: #2d3748; border: 1px solid #4a5568; color: #e2e8f0; font-size: 0.75rem; padding: 6px 10px; border-radius: 16px; cursor: pointer; }
    .preset-pill:hover { background: #4a5568; border-color: #ffcb05; }
    
    /* Console log */
    .console-box { background: #000; color: #39ff14; font-family: monospace; font-size: 0.75rem; padding: 10px; border-radius: 8px; height: 140px; overflow-y: auto; border: 1px solid #2f3642; }
    
    /* Switch */
    .switch-label { display: flex; align-items: center; justify-content: space-between; cursor: pointer; }
  </style>
</head>
<body>
  <header>
    <h1>GB POKÉMON TRADER</h1>
    <div class="subtitle">ESP32-S3 Game Boy Link Trade Station</div>
  </header>

  <div class="status-card">
    <div class="status-left">
      <div class="status-dot" id="statusDot"></div>
      <div>
        <div class="status-title" id="statusTitle">Desconectado</div>
        <div class="status-desc" id="statusDesc">Esperando que conectes el cable Link</div>
      </div>
    </div>
    <div class="gen-badge" id="genBadge">GEN I</div>
  </div>

  <div class="tabs">
    <button class="tab-btn active" onclick="switchTab('tab-inject')">⚡ Enviar PKMN</button>
    <button class="tab-btn" onclick="switchTab('tab-receive')">🔄 Recibido</button>
    <button class="tab-btn" onclick="switchTab('tab-monitor')">📡 Monitor</button>
    <button class="tab-btn" onclick="switchTab('tab-config')">⚙️ Ajustes</button>
  </div>

  <!-- TAB 1: INYECTOR / CREADOR -->
  <div id="tab-inject" class="tab-pane active">
    <div class="card">
      <div class="card-title">🚀 Presets Rápidos</div>
      <div class="presets">
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
      <div class="card-title">📝 Configurar Pokémon de Salida</div>
      <form id="pkmnForm" onsubmit="savePokemon(event)">
        <div class="grid-2">
          <div class="form-group">
            <label>Especie (ID Pokedex)</label>
            <input type="number" id="species" min="1" max="251" value="94" required onchange="onSpeciesChange()">
          </div>
          <div class="form-group">
            <label>Nivel (1 - 100)</label>
            <input type="number" id="level" min="1" max="100" value="50" required>
          </div>
        </div>

        <div class="grid-2">
          <div class="form-group">
            <label>Mote (Nickname)</label>
            <input type="text" id="nickname" maxlength="10" value="GENGAR">
          </div>
          <div class="form-group">
            <label>Entrenador (OT Name)</label>
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
            <select id="eviv">
              <option value="5" selected>Max IV & Max EV</option>
              <option value="3">Max IV, Zero EV</option>
              <option value="0">Random IV, Zero EV</option>
              <option value="2">Random IV, Max EV</option>
            </select>
          </div>
        </div>

        <div class="grid-2">
          <div class="form-group">
            <label>Movimiento 1 (ID)</label>
            <input type="number" id="m0" min="0" max="251" value="138">
          </div>
          <div class="form-group">
            <label>Movimiento 2 (ID)</label>
            <input type="number" id="m1" min="0" max="251" value="85">
          </div>
        </div>
        <div class="grid-2">
          <div class="form-group">
            <label>Movimiento 3 (ID)</label>
            <input type="number" id="m2" min="0" max="251" value="94">
          </div>
          <div class="form-group">
            <label>Movimiento 4 (ID)</label>
            <input type="number" id="m3" min="0" max="251" value="69">
          </div>
        </div>

        <div class="form-group" id="gen2Options" style="display:none; background:#181b20; padding:10px; border-radius:6px; margin-top:8px;">
          <div class="switch-label">
            <label style="margin:0;">✨ Variocolor (Shiny - Gen II)</label>
            <input type="checkbox" id="shiny" style="width:20px; height:20px;">
          </div>
          <div class="form-group" style="margin-top:10px;">
            <label>Objeto Equipado (Item ID)</label>
            <input type="number" id="item" min="0" max="255" value="0">
          </div>
        </div>

        <button type="submit" class="btn btn-primary" style="margin-top:8px;">💾 Cargar en la Mesa de Trade</button>
      </form>
    </div>
  </div>

  <!-- TAB 2: RECIBIDO / MODIFICAR -->
  <div id="tab-receive" class="tab-pane">
    <div class="card">
      <div class="card-title">📥 Pokémon Recibido de la Game Boy</div>
      <div id="noReceivedNotice" style="color:var(--text-muted); font-size:0.85rem;">
        Aún no se ha completado un intercambio. Cuando la Game Boy envíe un Pokémon, sus estadísticas y movimientos aparecerán aquí para ser modificados.
      </div>
      <div id="receivedData" style="display:none;">
        <div class="grid-2">
          <div class="form-group">
            <label>Especie</label>
            <input type="text" id="rec_species" readonly>
          </div>
          <div class="form-group">
            <label>Nivel Actual</label>
            <input type="number" id="rec_level" min="1" max="100">
          </div>
        </div>
        <div class="form-group">
          <label>Mote</label>
          <input type="text" id="rec_nick" maxlength="10">
        </div>
        <div class="grid-2">
          <div class="form-group">
            <label>Movimiento 1</label>
            <input type="number" id="rec_m0">
          </div>
          <div class="form-group">
            <label>Movimiento 2</label>
            <input type="number" id="rec_m1">
          </div>
        </div>
        <div class="grid-2">
          <div class="form-group">
            <label>Movimiento 3</label>
            <input type="number" id="rec_m2">
          </div>
          <div class="form-group">
            <label>Movimiento 4</label>
            <input type="number" id="rec_m3">
          </div>
        </div>
        <div class="switch-label" style="margin: 12px 0;">
          <label style="margin:0;">Hacer Shiny al devolver</label>
          <input type="checkbox" id="rec_shiny" style="width:20px; height:20px;">
        </div>
        <button type="button" class="btn btn-success" onclick="returnModifiedPokemon()">📤 Preparar para Devolver a Game Boy</button>
      </div>
    </div>
  </div>

  <!-- TAB 3: MONITOR EN VIVO -->
  <div id="tab-monitor" class="tab-pane">
    <div class="card">
      <div class="card-title">📡 Registro de Eventos del Cable Link</div>
      <div class="console-box" id="consoleLog">
        [00:00:00] Sistema iniciado. En espera de Game Boy...<br>
      </div>
    </div>
  </div>

  <!-- TAB 4: AJUSTES -->
  <div id="tab-config" class="tab-pane">
    <div class="card">
      <div class="card-title">⚙️ Generación de Juego</div>
      <div class="form-group">
        <label>Generación Destino</label>
        <select id="genSelect" onchange="changeGeneration()">
          <option value="1" selected>Generación I (Rojo / Azul / Amarillo)</option>
          <option value="2">Generación II (Oro / Plata / Cristal)</option>
        </select>
      </div>
      <div style="font-size:0.8rem; color:var(--text-muted); line-height: 1.4;">
        * Gen I puede intercambiar con Gen I y con Gen II (mediante la Cápsula del Tiempo).<br>
        * Gen II incluye Shinies, Objetos equipados y 251 especies.
      </div>
    </div>

    <div class="card">
      <div class="card-title">🔌 Conexionado ESP32-S3 SuperMini</div>
      <div style="font-size:0.8rem; color:var(--text-muted); line-height: 1.5;">
        <strong>Pines Predeterminados:</strong><br>
        • <strong>CLK (Pin 5 Game Boy):</strong> GPIO 4 (Divisor 5V ➔ 3.3V)<br>
        • <strong>SO (Pin 2 Game Boy):</strong> GPIO 5 (Divisor 5V ➔ 3.3V)<br>
        • <strong>SI (Pin 3 Game Boy):</strong> GPIO 6 (Directo 3.3V)<br>
        • <strong>GND (Pin 6 Game Boy):</strong> Masa común con ESP32 GND.<br>
        • <strong>Alimentación:</strong> USB-C del ESP32.
      </div>
    </div>
  </div>

  <script>
    let currentGen = 1;
    let ws;

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
        const dot = document.getElementById('statusDot');
        const title = document.getElementById('statusTitle');
        const desc = document.getElementById('statusDesc');

        title.textContent = data.title;
        desc.textContent = data.desc;

        dot.style.background = data.connected ? 'var(--badge-green)' : 'var(--badge-red)';
        log(`${data.title}: ${data.desc}`);
      } else if (data.type === 'received_pokemon') {
        document.getElementById('noReceivedNotice').style.display = 'none';
        document.getElementById('receivedData').style.display = 'block';

        document.getElementById('rec_species').value = data.name + ' (ID ' + data.species + ')';
        document.getElementById('rec_level').value = data.level;
        document.getElementById('rec_nick').value = data.nickname;
        document.getElementById('rec_m0').value = data.moves[0];
        document.getElementById('rec_m1').value = data.moves[1];
        document.getElementById('rec_m2').value = data.moves[2];
        document.getElementById('rec_m3').value = data.moves[3];
        log(`Recibido de Game Boy: ${data.name} (Lvl ${data.level})`);
      }
    }

    function switchTab(tabId) {
      document.querySelectorAll('.tab-pane').forEach(el => el.classList.remove('active'));
      document.querySelectorAll('.tab-btn').forEach(el => el.classList.remove('active'));
      document.getElementById(tabId).classList.add('active');
      event.target.classList.add('active');
    }

    function changeGeneration() {
      currentGen = parseInt(document.getElementById('genSelect').value);
      document.getElementById('genBadge').textContent = currentGen === 1 ? 'GEN I' : 'GEN II';
      document.getElementById('gen2Options').style.display = currentGen === 2 ? 'block' : 'none';
      fetch('/api/generation', {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify({ gen: currentGen })
      });
      log(`Cambiado a Generación ${currentGen === 1 ? 'I' : 'II'}`);
    }

    function onSpeciesChange() {
      const sp = parseInt(document.getElementById('species').value);
      if (sp === 94) document.getElementById('nickname').value = 'GENGAR';
      if (sp === 68) document.getElementById('nickname').value = 'MACHAMP';
      if (sp === 65) document.getElementById('nickname').value = 'ALAKAZAM';
      if (sp === 76) document.getElementById('nickname').value = 'GOLEM';
      if (sp === 151) document.getElementById('nickname').value = 'MEW';
      if (sp === 251) document.getElementById('nickname').value = 'CELEBI';
    }

    function applyPreset(preset) {
      if (preset === 'gengar') {
        document.getElementById('species').value = 94;
        document.getElementById('nickname').value = 'GENGAR';
        document.getElementById('level').value = 50;
        document.getElementById('m0').value = 138; // Dream Eater
        document.getElementById('m1').value = 85;  // Thunderbolt
        document.getElementById('m2').value = 94;  // Psychic
        document.getElementById('m3').value = 69;  // Seismic Toss
      } else if (preset === 'machamp') {
        document.getElementById('species').value = 68;
        document.getElementById('nickname').value = 'MACHAMP';
        document.getElementById('level').value = 50;
        document.getElementById('m0').value = 22; // Karate Chop
        document.getElementById('m1').value = 70; // Strength
        document.getElementById('m2').value = 90; // Fissure
        document.getElementById('m3').value = 25; // Mega Kick
      } else if (preset === 'alakazam') {
        document.getElementById('species').value = 65;
        document.getElementById('nickname').value = 'ALAKAZAM';
        document.getElementById('level').value = 50;
        document.getElementById('m0').value = 94; // Psychic
        document.getElementById('m1').value = 105; // Recover
        document.getElementById('m2').value = 115; // Reflect
        document.getElementById('m3').value = 129; // Swift
      } else if (preset === 'golem') {
        document.getElementById('species').value = 76;
        document.getElementById('nickname').value = 'GOLEM';
        document.getElementById('level').value = 50;
        document.getElementById('m0').value = 89; // Earthquake
        document.getElementById('m1').value = 88; // Rock Slide
        document.getElementById('m2').value = 153; // Explosion
        document.getElementById('m3').value = 23; // Body Slam
      } else if (preset === 'steelix') {
        document.getElementById('genSelect').value = 2;
        changeGeneration();
        document.getElementById('species').value = 208;
        document.getElementById('nickname').value = 'STEELIX';
        document.getElementById('level').value = 50;
        document.getElementById('m0').value = 231; // Iron Tail
        document.getElementById('m1').value = 89;  // Earthquake
        document.getElementById('m2').value = 157; // Rock Slide
        document.getElementById('m3').value = 23;  // Crunch
      } else if (preset === 'scizor') {
        document.getElementById('genSelect').value = 2;
        changeGeneration();
        document.getElementById('species').value = 212;
        document.getElementById('nickname').value = 'SCIZOR';
        document.getElementById('level').value = 50;
        document.getElementById('m0').value = 232; // Metal Claw
        document.getElementById('m1').value = 14;  // Swords Dance
        document.getElementById('m2').value = 97;  // Agility
        document.getElementById('m3').value = 210; // Fury Cutter
      } else if (preset === 'kingdra') {
        document.getElementById('genSelect').value = 2;
        changeGeneration();
        document.getElementById('species').value = 230;
        document.getElementById('nickname').value = 'KINGDRA';
        document.getElementById('level').value = 50;
        document.getElementById('m0').value = 56;  // Hydro Pump
        document.getElementById('m1').value = 225; // DragonBreath
        document.getElementById('m2').value = 82;  // Dragon Rage
        document.getElementById('m3').value = 61;  // Ice Beam
      } else if (preset === 'mew') {
        document.getElementById('species').value = 151;
        document.getElementById('nickname').value = 'MEW';
        document.getElementById('ot_name').value = 'MYSTRY';
        document.getElementById('ot_id').value = 69;
        document.getElementById('level').value = 30;
        document.getElementById('m0').value = 1;   // Pound
        document.getElementById('m1').value = 104; // Transform
        document.getElementById('m2').value = 129; // Swift
        document.getElementById('m3').value = 94;  // Psychic
      } else if (preset === 'celebi') {
        document.getElementById('genSelect').value = 2;
        changeGeneration();
        document.getElementById('species').value = 251;
        document.getElementById('nickname').value = 'CELEBI';
        document.getElementById('ot_name').value = 'WIN2011';
        document.getElementById('ot_id').value = 1121;
        document.getElementById('level').value = 50;
        document.getElementById('m0').value = 73;  // Leech Seed
        document.getElementById('m1').value = 105; // Recover
        document.getElementById('m2').value = 113; // Heal Bell
        document.getElementById('m3').value = 247; // AncientPower
      }
      log(`Preset aplicado: ${preset}`);
    }

    function savePokemon(e) {
      e.preventDefault();
      const payload = {
        species: parseInt(document.getElementById('species').value) - 1,
        level: parseInt(document.getElementById('level').value),
        nickname: document.getElementById('nickname').value,
        ot_name: document.getElementById('ot_name').value,
        ot_id: parseInt(document.getElementById('ot_id').value),
        m0: parseInt(document.getElementById('m0').value),
        m1: parseInt(document.getElementById('m1').value),
        m2: parseInt(document.getElementById('m2').value),
        m3: parseInt(document.getElementById('m3').value),
        eviv: parseInt(document.getElementById('eviv').value),
        shiny: document.getElementById('shiny').checked,
        item: parseInt(document.getElementById('item').value) || 0
      };

      fetch('/api/configure', {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify(payload)
      }).then(() => {
        alert('¡Pokémon cargado en la mesa de trade con éxito!');
        log(`Pokémon configurado: ${payload.nickname} (Nivel ${payload.level})`);
      });
    }

    function returnModifiedPokemon() {
      const payload = {
        level: parseInt(document.getElementById('rec_level').value),
        nickname: document.getElementById('rec_nick').value,
        m0: parseInt(document.getElementById('rec_m0').value),
        m1: parseInt(document.getElementById('rec_m1').value),
        m2: parseInt(document.getElementById('rec_m2').value),
        m3: parseInt(document.getElementById('rec_m3').value),
        shiny: document.getElementById('rec_shiny').checked,
        item: 0
      };

      fetch('/api/return_traded', {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify(payload)
      }).then(() => {
        alert('¡Pokémon preparado para devolver a la Game Boy!');
        log(`Devolución preparada: ${payload.nickname}`);
      });
    }

    window.onload = () => {
      initWS();
    };
  </script>
</body>
</html>
)rawliteral";
