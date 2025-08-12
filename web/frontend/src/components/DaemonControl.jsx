import React from 'react';

export default function DaemonControls({ handleStop, handleRestart, isStopping, isRestarting, isConnected }) {
  return (
    <div className="flex justify-end pt-4 space-x-4">
      <button
        onClick={handleStop}
        disabled={!isConnected || isStopping}
        className="p-3 px-6 border-2 border-yellow-500 text-yellow-500 hover:bg-yellow-500 hover:text-black transition-colors duration-200 disabled:border-gray-600 disabled:text-gray-600 disabled:hover:bg-transparent w-full sm:w-auto"
      >
        {isStopping ? 'Stopping...' : 'Stop Daemon'}
      </button>
      <button
        onClick={handleRestart}
        disabled={!isConnected || isRestarting}
        className="p-3 px-6 border-2 border-red-500 text-red-500 hover:bg-red-500 hover:text-black transition-colors duration-200 disabled:border-gray-600 disabled:text-gray-600 disabled:hover:bg-transparent w-full sm:w-auto"
      >
        {isRestarting ? 'Restarting...' : 'Restart Daemon'}
      </button>
    </div>
  );
}
