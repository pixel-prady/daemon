import React, { useRef, useEffect } from 'react';

export default function DaemonLog({ log, isConnected }) {
  const logRef = useRef(null);

  useEffect(() => {
    if (logRef.current) {
      logRef.current.scrollTop = logRef.current.scrollHeight;
    }
  }, [log]);

  return (
    <div className="h-64 overflow-y-auto pr-4 text-sm" ref={logRef}>
      {log.length === 0 ? (
        <p className="text-gray-500">
          <span className="text-green-400">$ </span>
          {isConnected ? 'Waiting for log entries...' : 'Connect to the daemon to view logs.'}
        </p>
      ) : (
        <ul>
          {log.map((entry, index) => (
            <li key={index} className="py-0.5">
              <span className="text-gray-500">[{entry.timestamp}]</span> <span className="text-green-400">$</span> {entry.message}
            </li>
          ))}
        </ul>
      )}
    </div>
  );
}
