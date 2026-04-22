#ifndef INTERFAZ_USUARIO_H
#define INTERFAZ_USUARIO_H

#include <Arduino.h>

const char PAGINA_HTML_PRINCIPAL[] PROGMEM = R"====(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <title>SAPO Monitoreo</title>
    <style>
        :root {
            --bg-color: #f2f2f7;
            --card-bg: rgba(255, 255, 255, 0.75);
            --primary: #007AFF;
            --danger: #FF3B30;
            --success: #34C759;
            --warning: #FF9500;
            --text-main: #1c1c1e;
            --text-muted: #8e8e93;
            --border-color: rgba(0, 0, 0, 0.05);
        }

        * { margin: 0; padding: 0; box-sizing: border-box; font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif; }

        body {
            background-color: var(--bg-color);
            color: var(--text-main);
            min-height: 100vh;
            padding: 2.5rem 1.5rem;
            display: flex;
            flex-direction: column;
            align-items: center;
            -webkit-font-smoothing: antialiased;
        }

        .header { text-align: center; margin-bottom: 2.5rem; }

        h1 {
            font-size: 2.5rem;
            font-weight: 700;
            letter-spacing: -0.5px;
            color: #000;
            margin-bottom: 0.2rem;
        }
        
        .subtitle {
            color: var(--text-muted);
            font-size: 1.1rem;
            font-weight: 500;
        }

        .dashboard {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(320px, 1fr));
            gap: 1.5rem;
            width: 100%;
            max-width: 1000px;
            margin-bottom: 1.5rem;
        }

        .card {
            background: var(--card-bg);
            backdrop-filter: blur(20px);
            -webkit-backdrop-filter: blur(20px);
            border: 1px solid var(--border-color);
            border-radius: 20px;
            padding: 1.5rem;
            box-shadow: 0 8px 24px rgba(0, 0, 0, 0.04);
            transition: transform 0.3s ease, box-shadow 0.3s ease;
        }

        .card:hover { transform: translateY(-4px); box-shadow: 0 12px 32px rgba(0, 0, 0, 0.08); }

        .card-header { display: flex; align-items: center; gap: 0.6rem; margin-bottom: 1rem; }
        
        .card-icon {
            width: 36px; height: 36px;
            border-radius: 10px;
            display: flex; align-items: center; justify-content: center;
            color: white;
            box-shadow: 0 4px 10px rgba(0,0,0,0.1);
        }
        .icon-blue { background: linear-gradient(135deg, #32ade6, #007aff); }
        .icon-purple { background: linear-gradient(135deg, #c644fc, #5856d6); }
        .icon-gray { background: linear-gradient(135deg, #aeaeb2, #8e8e93); }

        .card h2 { font-size: 1.1rem; color: var(--text-main); font-weight: 600; }

        .value-wrapper { margin-bottom: 1rem; display: flex; align-items: baseline;}
        .value-display { font-size: 3.5rem; font-weight: 700; line-height: 1; letter-spacing: -1px; color: #000; }
        .value-unit { font-size: 1.2rem; color: var(--text-muted); font-weight: 600; margin-left: 2px;}
        .value-secondary { font-size: 0.95rem; color: var(--text-muted); margin-left: 0.8rem; font-weight: 500; background: rgba(0,0,0,0.04); padding: 0.3rem 0.6rem; border-radius: 8px; border: 1px solid rgba(0,0,0,0.05); }

        .status-badge {
            display: inline-flex;
            align-items: center;
            gap: 0.4rem;
            padding: 0.4rem 0.8rem;
            border-radius: 8px;
            font-size: 0.85rem;
            font-weight: 600;
            background: rgba(52, 199, 89, 0.1);
            color: var(--success);
            transition: all 0.3s ease;
        }

        .status-badge.danger {
            background: rgba(255, 59, 48, 0.15);
            color: var(--danger);
            animation: pulse-danger 1.5s infinite;
        }

        .status-dot { width: 8px; height: 8px; border-radius: 50%; background-color: currentColor; }

        @keyframes pulse-danger {
            0% { opacity: 1; }
            50% { opacity: 0.5; }
            100% { opacity: 1; }
        }

        .stats-row {
            display: flex; justify-content: space-between; align-items: center;
            padding: 0.8rem 1rem; background: rgba(0, 0, 0, 0.03);
            border-radius: 12px; margin-bottom: 1rem;
        }

        .stats-label { color: var(--text-muted); font-size: 0.95rem; font-weight: 500;}
        .stats-value { font-size: 1.2rem; font-weight: 700; color: #000; }

        button {
            width: 100%; padding: 1rem; border-radius: 14px; font-size: 1rem;
            font-weight: 600; cursor: pointer; border: none; transition: all 0.2s ease;
            display: flex; justify-content: center; align-items: center; gap: 0.5rem;
        }

        .btn-danger { background: var(--danger); color: white; box-shadow: 0 4px 15px rgba(255,59,48,0.3); }
        .btn-danger:hover:not(:disabled) { background: #d70015; transform: scale(1.02); }
        .btn-danger:disabled { background: #e5e5ea; color: #c7c7cc; cursor: not-allowed; box-shadow: none; }

        .btn-outline { background: white; color: var(--text-main); border: 1px solid #d1d1d6; margin-top: 0.8rem; }
        .btn-outline:hover { background: #f2f2f7; }

        .chart-container { width: 100%; height: 380px; padding: 1.5rem; }
        .chart-wrapper { width: 100%; height: calc(100% - 2rem); position: relative; margin-top: 0.8rem; }
        canvas { width: 100%; height: 100%; }
        
        .cooldown-text { color: var(--warning); font-size: 0.85rem; font-weight: 500; text-align: center; margin-top: 0.5rem; display: none;}

        .bottom-grid {
            display: grid; grid-template-columns: 180px 1fr; gap: 1.5rem;
            width: 100%; max-width: 1000px;
        }

        .legend-card { display: flex; flex-direction: column; align-items: center; text-align: center; }
        .legend-scale-v { display: flex; gap: 1rem; align-items: stretch; height: 260px; margin-top: 1rem; width: 100%; justify-content: center; }
        .scale-bar-v { width: 12px; border-radius: 6px; overflow: hidden; display: flex; flex-direction: column; box-shadow: inset 0 2px 4px rgba(0,0,0,0.05); }
        .scale-segment-v.peligro { background: linear-gradient(180deg, #8b0000, var(--danger)); height: 51%; }
        .scale-segment-v.moderado { background: var(--warning); height: 34%; }
        .scale-segment-v.optimo { background: var(--success); height: 15%; }
        
        .scale-labels-v { position: relative; width: 60px; font-size: 0.85rem; color: var(--text-muted); font-weight: 600; text-align: right; }
        .scale-labels-v span { position: absolute; right: 0; transform: translateY(-50%); line-height: 1.2; }
        .scale-labels-v span.lbl-100 { top: 0%; }
        .scale-labels-v span.lbl-49 { top: 51%; color: var(--danger); }
        .scale-labels-v span.lbl-15 { top: 85%; }
        .scale-labels-v span.lbl-0 { top: 100%; }
        
        @media (max-width: 768px) {
            body { padding: 1rem 0.5rem; }
            .dashboard { grid-template-columns: 1fr; gap: 1rem; }
            .bottom-grid { grid-template-columns: 1fr; gap: 1rem; }
            .legend-scale-v { height: 200px; }
            .chart-container { height: 300px; }
            .card { padding: 1.2rem; border-radius: 16px;}
        }

        /* Custom Scrollbar */
    </style>
</head>
<body>

    <div class="header">
        <h1>S.A.P.O.</h1>
        <div class="subtitle">Monitoreo Ambiental Activo</div>
    </div>

    <div class="dashboard">
        <!-- Sensor Humo -->
        <div class="card" id="card-humo">
            <div class="card-header">
                <div class="card-icon icon-blue">
                    <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><path d="M17.5 19c-1.5 0-2.5-1-2.5-2.5C15 15 17 14 17 14s2 1 2 2.5a2.5 2.5 0 0 1-2.5 2.5z"/><path d="M6.5 17c-1.5 0-2.5-1-2.5-2.5C4 13 6 12 6 12s2 1 2 2.5A2.5 2.5 0 0 1 6.5 17z"/><path d="M12 22c-2.2 0-4-1.8-4-4 0-2 4-6 4-6s4 4 4 6c0 2.2-1.8 4-4 4z"/></svg>
                </div>
                <h2>Gases y Humo</h2>
            </div>
            <div class="value-wrapper">
                <span class="value-display" id="val-humo">0</span><span class="value-unit">%</span>
                <span class="value-secondary" id="ppm-humo">0 ppm</span>
            </div>
            <div id="badge-humo" class="status-badge">
                <div class="status-dot"></div><span id="txt-humo">Óptimo</span>
            </div>
        </div>

        <!-- Sensor Aire -->
        <div class="card" id="card-aire">
            <div class="card-header">
                <div class="card-icon icon-purple">
                    <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><path d="M9.5 4.5A2 2 0 1 1 11 8H2m10.5 11.5A2 2 0 1 0 14 16H2m15.5-8.5A2.5 2.5 0 1 1 19.5 12H2"/></svg>
                </div>
                <h2>Calidad de Aire</h2>
            </div>
            <div class="value-wrapper">
                <span class="value-display" id="val-aire">0</span><span class="value-unit">%</span>
                <span class="value-secondary" id="ppm-aire">0 ppm</span>
            </div>
            <div id="badge-aire" class="status-badge">
                <div class="status-dot"></div><span id="txt-aire">Saludable</span>
            </div>
        </div>

        <!-- Panel -->
        <div class="card">
            <div class="card-header">
                <div class="card-icon icon-gray">
                    <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="3"></circle><path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1 0 2.83 2 2 0 0 1-2.83 0l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-2 2 2 2 0 0 1-2-2v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83 0 2 2 0 0 1 0-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1-2-2 2 2 0 0 1 2-2h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 0-2.83 2 2 0 0 1 2.83 0l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 2-2 2 2 0 0 1 2 2v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 0 2 2 0 0 1 0 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 2 2 2 2 0 0 1-2 2h-.09a1.65 1.65 0 0 0-1.51 1z"></path></svg>
                </div>
                <h2>Panel Principal</h2>
            </div>
            
            <div class="stats-row">
                <div class="stats-label">Alertas Históricas</div>
                <div class="stats-value" id="val-contador">0</div>
            </div>

            <button id="btn-apagar" class="btn-danger" onclick="apagarAlarma()" disabled>Detener Alarma</button>
            <div id="txt-cooldown" class="cooldown-text">¡Sistema en espera (30s)!</div>
            
            <button class="btn-outline" onclick="borrarHistorial()">Limpiar Registros</button>
        </div>
    </div>

    <div class="bottom-grid">
        <!-- Tarjeta: Indicador de Índice de Calidad -->
        <div class="card legend-card">
            <h2>Índice de Calidad</h2>
            <div class="legend-scale-v">
                <div class="scale-labels-v">
                    <span class="lbl-100">100%<br>Tóx.</span>
                    <span class="lbl-49">49%<br>Pelig.</span>
                    <span class="lbl-15">15%<br>Acept.</span>
                    <span class="lbl-0">0%<br>Ópt.</span>
                </div>
                <div class="scale-bar-v">
                    <div class="scale-segment-v peligro"></div>
                    <div class="scale-segment-v moderado"></div>
                    <div class="scale-segment-v optimo"></div>
                </div>
            </div>
        </div>

        <!-- Tarjeta: Analytics -->
        <div class="card chart-container">
            <div class="card-header" style="margin-bottom:0.2rem">
                <div class="card-icon icon-blue" style="width:28px;height:28px">
                    <svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><polyline points="22 12 18 12 15 21 9 3 6 12 2 12"></polyline></svg>
                </div>
                <h2>Histórico de Semana</h2>
            </div>
            
            <div class="chart-wrapper">
                <canvas id="grafica"></canvas>
            </div>
        </div>
    </div>

    <script>
        // Mapeo amigable para el humano (0% al 100%)
        const mapPct = (adc) => Math.min(100, Math.round((adc / 4095) * 100));
        const UMBRAL_ADC = 2000;
        const UMBRAL_PCT = mapPct(UMBRAL_ADC);

        let alarmaActiva = false;
        let cooldownActivo = false;

        async function fetchUIUpdate() {
            try {
                const res = await fetch('/api/datos');
                const data = await res.json();
                
                const pctHumo = mapPct(data.humo);
                const pctAire = mapPct(data.aire);

                document.getElementById('val-humo').innerText = pctHumo;
                document.getElementById('val-aire').innerText = pctAire;
                
                // Mostrar equivalencia aproximada en Partes Por Millon
                document.getElementById('ppm-humo').innerText = data.humo + ' ppm';
                document.getElementById('ppm-aire').innerText = data.aire + ' ppm';
                
                document.getElementById('val-contador').innerText = data.alarmas;

                actualizarEstado('humo', data.humo, pctHumo);
                actualizarEstado('aire', data.aire, pctAire);

                alarmaActiva = data.alarma_activa === 1;
                document.getElementById('btn-apagar').disabled = !alarmaActiva;

            } catch(e) {}
        }

        setInterval(fetchUIUpdate, 1000);

        function actualizarEstado(tipo, adc, pct) {
            const badge = document.getElementById(`badge-${tipo}`);
            const txt = document.getElementById(`txt-${tipo}`);

            if(adc >= UMBRAL_ADC) {
                badge.className = 'status-badge danger';
                txt.innerText = 'Peligro Activo';
            } else if (pct > 30) {
                badge.className = 'status-badge';
                badge.style.color = 'var(--warning)';
                badge.style.background = 'rgba(255, 149, 0, 0.1)';
                txt.innerText = 'Moderado';
            } else {
                badge.className = 'status-badge';
                badge.style.color = 'var(--success)';
                badge.style.background = 'rgba(52, 199, 89, 0.1)';
                txt.innerText = tipo === 'humo' ? 'Óptimo' : 'Saludable';
            }
        }

        async function apagarAlarma() {
            try { 
                await fetch('/api/apagar', {method: 'POST'}); 
                // UI feedback instantaneo
                document.getElementById('btn-apagar').disabled = true;
                const cdt = document.getElementById('txt-cooldown');
                cdt.style.display = 'block';
                setTimeout(() => cdt.style.display = 'none', 30000); // 30s ocultar texto
            } 
            catch(e) {}
        }

        async function borrarHistorial() {
            if(confirm("¿Borrar todos los datos permanentemente?")) {
                try { 
                    await fetch('/api/borrar_historial', {method: 'POST'});
                    cargarGrafica(); 
                } catch(e) {}
            }
        }

        async function cargarGrafica() {
            try {
                const res = await fetch('/api/historial');
                const datos = await res.json();
                dibujarGrafica(datos);
            } catch(e) {}
        }

        function dibujarGrafica(datos) {
            const canvas = document.getElementById('grafica');
            const cw = canvas.parentElement.clientWidth;
            const ch = canvas.parentElement.clientHeight;
            
            const dpr = window.devicePixelRatio || 1;
            canvas.width = cw * dpr;
            canvas.height = ch * dpr;
            
            const ctx = canvas.getContext('2d');
            ctx.scale(dpr, dpr);
            ctx.clearRect(0, 0, cw, ch);
            
            if(datos.length === 0) {
                ctx.fillStyle = '#8e8e93';
                ctx.font = '500 15px sans-serif';
                ctx.textAlign = 'center';
                ctx.fillText('Recopilando datos...', cw/2, ch/2);
                return;
            }

            const pb = 25; const pl = 30; 
            const w = cw - pl - 10;
            const h = ch - pb - 15;
            const MAX_VAL = 100; // Trabajando en 100%
            const dx = datos.length > 1 ? w / (datos.length - 1) : w;

            // Guias Y
            ctx.fillStyle = '#8e8e93';
            ctx.font = '12px sans-serif';
            ctx.textAlign = 'right';
            ctx.fillText('100%', pl - 5, 10);
            ctx.fillText('0%', pl - 5, ch - pb);

            // Guias X (Cálculo de Fechas off-line usando salto relativo)
            const lastDataTimeMs = datos[datos.length - 1].time;
            const now = new Date();
            
            ctx.textAlign = 'center';
            const numLabels = 5; // Dibujar 5 etiquetas en el eje X
            for(let i=0; i<numLabels; i++) {
                const docIdx = Math.floor(i * (datos.length - 1) / (numLabels - 1));
                if(datos[docIdx]) {
                    const diffMs = lastDataTimeMs - datos[docIdx].time;
                    const realDate = new Date(now.getTime() - diffMs);
                    
                    // Formato ej: "14:30" o "Mar 20, 14:30"
                    const horaMin = realDate.getHours().toString().padStart(2, '0') + ':' + realDate.getMinutes().toString().padStart(2, '0');
                    
                    let xPos = pl + (docIdx * dx);
                    if(i === 0) { ctx.textAlign = 'left'; xPos += 2; }
                    else if(i === numLabels - 1) { ctx.textAlign = 'right'; xPos -= 2; }
                    else { ctx.textAlign = 'center'; }
                    
                    ctx.fillText(horaMin, xPos, ch - 5);
                }
            }

            const trazarData = (key, hex) => {
                ctx.beginPath();
                ctx.lineWidth = 3;
                ctx.strokeStyle = hex;
                ctx.lineJoin = 'round';
                
                datos.forEach((d, i) => {
                    const valPct = mapPct(d[key]);
                    const x = pl + (i * dx);
                    const y = (ch - pb) - (valPct / MAX_VAL) * h;
                    if(i === 0) ctx.moveTo(x, y);
                    else ctx.lineTo(x, y);
                });
                ctx.stroke();
            };

            // Umbral
            const yUmbral = (ch - pb) - (UMBRAL_PCT / MAX_VAL) * h;
            ctx.beginPath();
            ctx.setLineDash([5, 5]);
            ctx.strokeStyle = 'rgba(255, 59, 48, 0.4)';
            ctx.moveTo(pl, yUmbral);
            ctx.lineTo(pl + w, yUmbral);
            ctx.stroke();
            ctx.setLineDash([]);

            trazarData('aire', '#5856d6'); 
            trazarData('humo', '#007aff'); 
        }

        cargarGrafica();
        setInterval(cargarGrafica, 600000);
        window.addEventListener('resize', cargarGrafica);
    </script>
</body>
</html>
)====";

#endif // INTERFAZ_USUARIO_H
