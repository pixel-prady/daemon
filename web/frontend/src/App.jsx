import React, { useState, useEffect, useRef } from 'react';
import DaemonLog from './components/DaemonLog.jsx';
import DaemonSettings from './components/DaemonSettings.jsx';
import DaemonControls from './components/DaemonControl.jsx';

const WS_URL = import.meta.env.WS_URL || "ws://localhost:8080";

export default function App() {
  const [daemonId, setDaemonId] = useState('');
  const [isConnected, setIsConnected] = useState(false);
  const [log, setLog] = useState([]);
  const [settings, setSettings] = useState({
    cpu_idle_threshold: 25,
    watch_path: '/home/zeus/Videos/AutoSortDaemon/watchfolder',
    segregate_existing_files: true,
  });
  const [inputSettings, setInputSettings] = useState(settings);
  const [isSaving, setIsSaving] = useState({});
  const [connectionError, setConnectionError] = useState('');
  const [cpuUsage, setCpuUsage] = useState(0);
  const [isRestarting, setIsRestarting] = useState(false);
  const [isStopping, setIsStopping] = useState(false);

  const ws = useRef(null);

  useEffect(() => {
    if (isConnected && daemonId) {
      if (ws.current) {
        ws.current.close();
      }

      ws.current = new WebSocket(WS_URL);

      ws.current.onopen = () => {
        ws.current.send(daemonId);
        setLog(prev => [...prev, { timestamp: new Date().toLocaleTimeString(), message: 'Connected to daemon.' }]);
      };

      ws.current.onmessage = (event) => {
        if (event.data instanceof Blob) {
          event.data.text().then(text => {
            setLog(prev => [...prev, { timestamp: new Date().toLocaleTimeString(), message: text }].slice(-50));
          });
        } else {
          setLog(prev => [...prev, { timestamp: new Date().toLocaleTimeString(), message: event.data }].slice(-50));
        }
      };

      ws.current.onclose = () => {
        setIsConnected(false);
      };

      ws.current.onerror = (error) => {
        setLog(prev => [...prev, { timestamp: new Date().toLocaleTimeString(), message: `WebSocket error: ${error.message}` }]);
      };

      return () => {
        if (ws.current) {
          ws.current.close();
          ws.current = null;
        }
      };
    }

    return () => {
      if (ws.current) {
        ws.current.close();
        ws.current = null;
      }
    };
  }, [isConnected, daemonId]);

  const handleConnect = () => {
    if (!isConnected && !daemonId) {
      setConnectionError('Please enter a Daemon ID to connect.');
      return;
    }
    setConnectionError('');
    if (isConnected && ws.current) {
      ws.current.close();
      setIsConnected(false);
      setLog([]);
      setCpuUsage(0);
    } else {
      setIsConnected(true);
    }
  };

  const handleRestart = () => {
    if (!isConnected) {
      setLog(prev => [...prev, { timestamp: new Date().toLocaleTimeString(), message: 'ERROR: Daemon is not connected.' }]);
      return;
    }
    setIsRestarting(true);
    if (ws.current && ws.current.readyState === WebSocket.OPEN) {
      ws.current.send('restart');
      setLog(prev => [...prev, { timestamp: new Date().toLocaleTimeString(), message: 'Restart command sent.' }]);
    }
  
    setIsRestarting(false);
  };

  const handleStop = () => {
    if (!isConnected) {
      setLog(prev => [...prev, { timestamp: new Date().toLocaleTimeString(), message: 'ERROR: Daemon is not connected.' }]);
      return;
    }
    setIsStopping(true);
    if (ws.current && ws.current.readyState === WebSocket.OPEN) {
      ws.current.send('stop');
      setLog(prev => [...prev, { timestamp: new Date().toLocaleTimeString(), message: 'Stop command sent.' }]);
    }
 
    setIsStopping(false);
    setIsConnected(false);
    setCpuUsage(0);
  };

  const handleCoreInputChange = (e) => {
    const { name, value, type, checked } = e.target;
    setInputSettings(prev => ({
      ...prev,
      [name]: type === 'checkbox' ? checked : value,
    }));
  };

  const handleUpdateSetting = (key) => {
    if (!ws.current || ws.current.readyState !== WebSocket.OPEN) {
      setLog(prev => [...prev, { timestamp: new Date().toLocaleTimeString(), message: 'ERROR: Not connected to daemon.' }]);
      return;
    }
    if (inputSettings[key] === settings[key]) {
      setLog(prev => [...prev, { timestamp: new Date().toLocaleTimeString(), message: `No changes for ${key}.` }]);
      return;
    }

    setIsSaving(prev => ({ ...prev, [key]: true }));

    const command = `update:${key}=${inputSettings[key]}`;
    ws.current.send(command);
    setLog(prev => [...prev, { timestamp: new Date().toLocaleTimeString(), message: `Sent command: ${command}` }]);

    setSettings(prev => ({ ...prev, [key]: inputSettings[key] }));
    setIsSaving(prev => ({ ...prev, [key]: false }));
  };

  return (
    <div className="min-h-screen bg-gray-900 text-green-400 p-8 font-mono antialiased flex flex-col items-center justify-center">
      <div className="w-full max-w-6xl bg-black border-2 border-green-400 shadow-[0_0_15px_rgba(34,197,94,0.5)] rounded-lg p-6 space-y-6 overflow-hidden">

        {/* Terminal Header */}
        <div className="flex justify-between items-center text-sm border-b-2 border-green-400 pb-2">
          <div className="flex items-center space-x-2">
            <span className={`w-3 h-3 rounded-full ${isConnected ? 'bg-green-500' : 'bg-red-500'} animate-pulse`}></span>
            <span className="uppercase font-bold">{isConnected ? 'STATUS: CONNECTED' : 'STATUS: DISCONNECTED'}</span>
          </div>
          <span className="text-gray-500">
            CPU: <span className="text-green-400">{cpuUsage}%</span>
          </span>
        </div>

        <DaemonLog log={log} isConnected={isConnected} />

        {/* Command Input and Connect Button */}
        <div className="flex items-center space-x-4">
          <span className="text-green-400 font-bold">$</span>
          <input
            type="text"
            placeholder="Enter Daemon ID"
            value={daemonId}
            onChange={(e) => setDaemonId(e.target.value)}
            className="flex-1 bg-transparent border-b-2 border-green-400 text-green-400 outline-none focus:border-green-300 transition-colors duration-200"
          />
          <button
            onClick={handleConnect}
            className={`px-4 py-2 border-2 ${isConnected ? 'border-red-500 text-red-500 hover:bg-red-500 hover:text-black' : 'border-green-400 text-green-400 hover:bg-green-400 hover:text-black'} transition-all duration-200 uppercase font-bold text-sm`}
          >
            {isConnected ? 'Disconnect' : 'Connect'}
          </button>
        </div>
        {connectionError && <p className="text-red-500 text-sm mt-2">{connectionError}</p>}

        <DaemonSettings
          inputSettings={inputSettings}
          handleCoreInputChange={handleCoreInputChange}
          handleUpdateSetting={handleUpdateSetting}
          isSaving={isSaving}
          isConnected={isConnected}
        />

        <DaemonControls
          handleStop={handleStop}
          handleRestart={handleRestart}
          isStopping={isStopping}
          isRestarting={isRestarting}
          isConnected={isConnected}
        />
      </div>
    </div>
  );
}
