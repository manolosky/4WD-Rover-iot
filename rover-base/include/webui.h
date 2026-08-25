#pragma once
#include <Arduino.h>

// ============================================================
// webui.h — Embedded web UI (joysticks + FPV + telemetry)
// Served from PROGMEM. Single page, no external dependencies.
// ============================================================

const char WEB_UI[] PROGMEM = R"HTML(
<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no">
<title>Rover 4WD</title>
<style>
:root{--bg:#0d0f14;--sf:#161922;--bd:#252a38;--tx:#e2e4ea;--mt:#7a7f8e;
--or:#f59e42;--gn:#4ade80;--rd:#f54262;--bl:#42b4f5}
*{margin:0;padding:0;box-sizing:border-box;-webkit-tap-highlight-color:transparent;user-select:none}
body{background:var(--bg);color:var(--tx);font-family:system-ui,sans-serif;
height:100dvh;display:flex;flex-direction:column;overflow:hidden;touch-action:none}
/* ---- Video ---- */
#videoWrap{position:relative;flex:1;background:#000;display:flex;align-items:center;justify-content:center;overflow:hidden}
#video{width:100%;height:100%;object-fit:cover}
#noCam{color:var(--mt);font-size:14px;text-align:center;padding:20px}
/* ---- HUD ---- */
#hud{position:absolute;top:8px;left:8px;right:8px;display:flex;justify-content:space-between;pointer-events:none}
.pill{background:rgba(13,15,20,.75);backdrop-filter:blur(6px);border-radius:20px;
padding:5px 12px;font-size:12px;font-weight:600;display:flex;align-items:center;gap:6px}
#batPill .dot{width:8px;height:8px;border-radius:50%;background:var(--gn)}
#batPill.low .dot{background:var(--or)}#batPill.crit .dot{background:var(--rd);animation:blink 1s infinite}
@keyframes blink{50%{opacity:.2}}
#alertPill{display:none;background:rgba(245,66,98,.85);color:#fff}
/* ---- Controls ---- */
#controls{display:flex;align-items:center;justify-content:space-between;
padding:12px 18px calc(14px + env(safe-area-inset-bottom));gap:10px;background:var(--sf);border-top:1px solid var(--bd)}
/* Two single-axis joysticks: vertical (throttle) and horizontal (steering) */
.joy{background:radial-gradient(circle,#1d2230,#161922);border:2px solid var(--bd);
position:relative;flex-shrink:0}
.joy.v{width:84px;height:170px;border-radius:42px}
.joy.h{width:170px;height:84px;border-radius:42px}
.knob{width:60px;height:60px;border-radius:50%;background:linear-gradient(145deg,var(--or),#d97f28);
position:absolute;left:50%;top:50%;transform:translate(-50%,-50%);box-shadow:0 3px 12px rgba(245,158,66,.4)}
#mid{display:flex;flex-direction:column;align-items:center;gap:10px}
#btns{display:flex;flex-direction:row;gap:10px}
/* Gear selector */
#gears{display:flex;gap:6px}
.gear{width:46px;height:42px;border-radius:12px;border:1px solid var(--bd);background:#1d2230;
color:var(--mt);font-size:15px;font-weight:700;transition:all .12s}
.gear.on{background:var(--gn);color:#0d0f14;border-color:var(--gn)}
.btn{width:64px;height:64px;border-radius:16px;border:1px solid var(--bd);background:#1d2230;
color:var(--tx);font-size:24px;display:flex;align-items:center;justify-content:center;transition:all .12s}
.btn:active{transform:scale(.92);background:#252b3d}
.btn.on{background:var(--or);color:#0d0f14;border-color:var(--or)}
#stats{font-size:11px;color:var(--mt);line-height:1.7;min-width:90px}
#stats b{color:var(--tx);font-size:13px}
</style>
</head>
<body>
<div id="videoWrap">
  <img id="video" style="display:none" alt="">
  <div id="noCam">📷 Esperando cámara FPV…<br><small>El video aparecerá automáticamente</small></div>
  <div id="hud">
    <div class="pill" id="batPill"><span class="dot"></span><span id="batTxt">--.-V</span></div>
    <div class="pill" id="alertPill">⚠️ <span id="alertTxt"></span></div>
    <div class="pill"><span id="spdTxt">0.00 m/s</span></div>
  </div>
</div>
<div id="controls">
  <div class="joy v" id="joyY"><div class="knob" id="knobY"></div></div>
  <div id="mid">
    <div id="stats">
      <div>RPM <b id="rpmTxt">0</b></div>
      <div>Dist <b id="distTxt">0 m</b></div>
      <div>Pitch <b id="pitchTxt">0°</b></div>
    </div>
    <div id="gears">
      <button class="gear" data-g="0" title="Lenta">1</button>
      <button class="gear on" data-g="1" title="Normal">2</button>
      <button class="gear" data-g="2" title="Rápida">3</button>
    </div>
    <div id="btns">
      <button class="btn" id="lightBtn" title="Luces">💡</button>
      <button class="btn" id="findBtn" title="Encontrar">📢</button>
    </div>
  </div>
  <div class="joy h" id="joyX"><div class="knob" id="knobX"></div></div>
</div>
<script>
// Two independent single-axis joysticks (multi-touch capable:
// each zone tracks its own finger via targetTouches)
let jx=0,jy=0;
function axis(zone,knob,horiz,cb){
  const R=(horiz?zone.clientWidth:zone.clientHeight)/2-34;
  function upd(t){
    const r=zone.getBoundingClientRect();
    let d=horiz? t.clientX-(r.left+r.width/2) : t.clientY-(r.top+r.height/2);
    d=Math.max(-R,Math.min(R,d));
    knob.style.transform=horiz?`translate(calc(-50% + ${d}px),-50%)`:`translate(-50%,calc(-50% + ${d}px))`;
    cb(Math.round((horiz?d:-d)/R*100));
  }
  function reset(){knob.style.transform='translate(-50%,-50%)';cb(0);send()}
  zone.addEventListener('touchstart',e=>{upd(e.targetTouches[0]);e.preventDefault()},{passive:false});
  zone.addEventListener('touchmove',e=>{upd(e.targetTouches[0]);e.preventDefault()},{passive:false});
  zone.addEventListener('touchend',e=>{if(0===e.targetTouches.length)reset()});
  let md=false;
  zone.addEventListener('mousedown',e=>{md=true;upd(e)});
  window.addEventListener('mousemove',e=>{if(md)upd(e)});
  window.addEventListener('mouseup',()=>{if(md){md=false;reset()}});
}
axis(document.getElementById('joyY'),document.getElementById('knobY'),false,v=>{jy=v});
axis(document.getElementById('joyX'),document.getElementById('knobX'),true,v=>{jx=v});

// Send commands at 15Hz (only on change, or every 400ms as keepalive)
let lastSent=0,lastX=0,lastY=0;
function send(){
  fetch(`/cmd?x=${jx}&y=${jy}`).catch(()=>{});
  lastSent=Date.now();lastX=jx;lastY=jy;
}
setInterval(()=>{
  if(jx!==lastX||jy!==lastY||Date.now()-lastSent>400) send();
},66);

// Gears
document.querySelectorAll('.gear').forEach(b=>b.onclick=()=>
  fetch('/gear?v='+b.dataset.g).then(()=>{
    document.querySelectorAll('.gear').forEach(x=>x.classList.toggle('on',x===b));
  }).catch(()=>{}));

// Buttons
const lightBtn=document.getElementById('lightBtn');
lightBtn.onclick=()=>fetch('/lights').then(r=>r.text()).then(s=>lightBtn.classList.toggle('on',s==='1'));
document.getElementById('findBtn').onclick=()=>fetch('/find');

// Telemetry
const batPill=document.getElementById('batPill');
setInterval(()=>fetch('/status').then(r=>r.json()).then(s=>{
  document.getElementById('batTxt').textContent=s.v.toFixed(1)+'V '+s.pct+'%';
  document.getElementById('spdTxt').textContent=s.spd.toFixed(2)+' m/s';
  document.getElementById('rpmTxt').textContent=Math.round(s.rpm);
  document.getElementById('distTxt').textContent=(s.dist/1000).toFixed(1)+' m';
  document.getElementById('pitchTxt').textContent=Math.round(s.pitch)+'°';
  batPill.classList.toggle('low',s.low&&!s.crit);
  batPill.classList.toggle('crit',s.crit);
  const a=document.getElementById('alertPill');
  if(s.state==='ROLLED'){a.style.display='flex';document.getElementById('alertTxt').textContent='VOLCADO'}
  else if(s.state==='LIFTED'){a.style.display='flex';document.getElementById('alertTxt').textContent='ALZADO'}
  else if(s.state==='TILT'){a.style.display='flex';document.getElementById('alertTxt').textContent='INCLINACIÓN'}
  else a.style.display='none';
}).catch(()=>{}),500);

// FPV video: try to connect to the camera, retry on failure.
// With CAM_ON=0 the camera stays on standby and no connection is opened.
const CAM_ON=%CAM_ON%;
const vid=document.getElementById('video'),noCam=document.getElementById('noCam');
function tryCam(){
  const img=new Image();
  img.onload=()=>{vid.src='%CAM_URL%';vid.style.display='block';noCam.style.display='none'};
  img.onerror=()=>setTimeout(tryCam,4000);
  img.src='%CAM_URL%';
}
if(CAM_ON)tryCam();
else noCam.innerHTML='📷 Cámara FPV en standby';
</script>
</body>
</html>
)HTML";
