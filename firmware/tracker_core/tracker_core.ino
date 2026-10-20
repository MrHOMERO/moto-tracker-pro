<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Dispositivo Tracker Pro (HW)</title>
    <script src="https://cdn.jsdelivr.net/npm/@tailwindcss/browser@4"></script>
    <!-- Firebase SDKs -->
    <script src="https://www.gstatic.com/firebasejs/10.8.0/firebase-app-compat.js"></script>
    <script src="https://www.gstatic.com/firebasejs/10.8.0/firebase-firestore-compat.js"></script>
</head>
<body class="bg-slate-950 text-slate-100 p-6 flex flex-col items-center justify-center min-h-screen font-sans">

    <div class="bg-slate-900 border border-slate-800 p-6 rounded-3xl max-w-sm w-full space-y-4 shadow-2xl text-center">
        <div class="inline-block p-3 bg-emerald-500/10 rounded-2xl border border-emerald-500/20 text-emerald-400 mb-2">
            📡 MÓDULO HARDWARE (SIMULADO)
        </div>
        <h1 class="text-xl font-bold text-white">Rastreador en Vehículo</h1>
        <p class="text-xs text-slate-400">Este celular está operando bajo el asiento.</p>

        <div class="bg-slate-950 p-4 rounded-2xl border border-slate-800 text-left space-y-2 text-xs font-mono">
            <p><strong>Estado:</strong> <span id="txt-estado" class="text-amber-400 font-semibold">Reposo (Deep Sleep)</span></p>
            <p><strong>GPS Lat:</strong> <span id="txt-lat" class="text-slate-400">---</span></p>
            <p><strong>GPS Lng:</strong> <span id="txt-lng" class="text-slate-400">---</span></p>
        </div>

        <button id="btn-toggle" onclick="encenderDispositivo()" class="w-full bg-emerald-600 hover:bg-emerald-500 text-white font-bold py-3 px-4 rounded-xl transition cursor-pointer">
            Encender Módulo
        </button>
    </div>

    <script>
        // Configuración oficial de tu Firebase
        const firebaseConfig = {
            apiKey: "AIzaSyBX3meGbx-DoOVXTYlCTRYI7ekNpGaDdyk",
            authDomain: "moto-tracker-pro-dd232.firebaseapp.com",
            projectId: "moto-tracker-pro-dd232",
            storageBucket: "moto-tracker-pro-dd232.firebasestorage.app",
            messagingSenderId: "456174033037",
            appId: "1:456174033037:web:ee2ae07303e6ba8789aac4",
            measurementId: "G-BPGF7FDQXX"
        };
        firebase.initializeApp(firebaseConfig);
        const db = firebase.firestore();

        let watchId = null;

        function encenderDispositivo() {
            document.getElementById('btn-toggle').disabled = true;
            document.getElementById('btn-toggle').innerText = "Módulo Activo (Escuchando)";
            document.getElementById('txt-estado').innerText = "Esperando alerta de robo...";

            // Escuchar si el celular principal activa el modo emergencia
            db.collection("tracker").doc("estado").onSnapshot((doc) => {
                if (doc.exists && doc.data().modoRobo === true) {
                    document.getElementById('txt-estado').innerText = "¡MODO RASTREO ACTIVADO!";
                    document.getElementById('txt-estado').className = "text-red-500 font-bold animate-pulse";
                    iniciarGPSReal();
                }
            });
        }

        function iniciarGPSReal() {
            if ('geolocation' in navigator) {
                if (watchId !== null) return;
                watchId = navigator.geolocation.watchPosition((pos) => {
                    const lat = pos.coords.latitude;
                    const lng = pos.coords.longitude;

                    document.getElementById('txt-lat').innerText = lat.toFixed(6);
                    document.getElementById('txt-lng').innerText = lng.toFixed(6);

                    // Enviar coordenadas a Firestore
                    db.collection("tracker").doc("ubicacion").set({
                        lat: lat,
                        lng: lng,
                        timestamp: Date.now()
                    });
                }, (err) => {
                    alert("Error de GPS: " + err.message);
                }, {
                    enableHighAccuracy: true,
                    maximumAge: 0,
                    timeout: 5000
                });
            } else {
                alert("Geolocalización no soportada en este navegador.");
            }
        }
    </script>
</body>
</html>
