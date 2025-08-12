import React from "react";

export default function DaemonSettings({
  inputSettings,
  handleCoreInputChange,
  handleUpdateSetting,
  isSaving,
  isConnected,
}) {
  return (
    <div className="border-t-2 border-green-400 pt-6 space-y-4">
      <h2 className="text-lg font-bold">DAEMON CONFIGURATION</h2>

      {/* CPU Idle Threshold */}
      <div className="flex flex-col sm:flex-row items-start sm:items-center space-y-2 sm:space-y-0 sm:space-x-4">
        <label htmlFor="cpu_idle_threshold" className="w-full sm:w-1/2">
          <span className="text-gray-500">cpu_idle_threshold</span> =
        </label>
        <input
          type="number"
          id="cpu_idle_threshold"
          name="cpu_idle_threshold"
          value={inputSettings.cpu_idle_threshold}
          onChange={handleCoreInputChange}
          className="flex-1 bg-transparent border-b border-gray-600 text-green-400 outline-none focus:border-green-400 w-full"
        />
        <button
          onClick={() => handleUpdateSetting("cpu_idle_threshold")}
          disabled={!isConnected || isSaving.cpu_idle_threshold}
          className="mt-2 sm:mt-0 p-2 px-4 border border-green-400 text-green-400 hover:bg-green-400 hover:text-black transition-colors duration-200 disabled:border-gray-600 disabled:text-gray-600 disabled:hover:bg-transparent w-full sm:w-auto"
        >
          {isSaving.cpu_idle_threshold ? "Updating..." : "Update"}
        </button>
      </div>

      {/* Watch Path */}
      <div className="flex flex-col sm:flex-row items-start sm:items-center space-y-2 sm:space-y-0 sm:space-x-4">
        <label htmlFor="watch_path" className="w-full sm:w-1/2">
          <span className="text-gray-500">watch_path</span> =
        </label>
        <input
          type="text"
          id="watch_path"
          name="watch_path"
          value={inputSettings.watch_path}
          onChange={handleCoreInputChange}
          className="flex-1 bg-transparent border-b border-gray-600 text-green-400 outline-none focus:border-green-400 w-full"
        />
        <button
          onClick={() => handleUpdateSetting("watch_path")}
          disabled={!isConnected || isSaving.watch_path}
          className="mt-2 sm:mt-0 p-2 px-4 border border-green-400 text-green-400 hover:bg-green-400 hover:text-black transition-colors duration-200 disabled:border-gray-600 disabled:text-gray-600 disabled:hover:bg-transparent w-full sm:w-auto"
        >
          {isSaving.watch_path ? "Updating..." : "Update"}
        </button>
      </div>

      {/* Segregate Existing Files */}
      <div className="flex flex-col sm:flex-row items-start sm:items-center space-y-2 sm:space-y-0 sm:space-x-4">
        <div className="flex items-center space-x-2 w-full sm:w-1/2">
          <input
            type="checkbox"
            id="segregate_existing_files"
            name="segregate_existing_files"
            checked={inputSettings.segregate_existing_files}
            onChange={handleCoreInputChange}
            className="h-4 w-4 bg-transparent border-green-400 text-green-400 focus:ring-green-400"
          />
          <label htmlFor="segregate_existing_files" className="text-green-400">
            <span className="text-gray-500">
              segregate_existing_files
            </span>
          </label>
        </div>
        <button
          onClick={() => handleUpdateSetting("segregate_existing_files")}
          disabled={!isConnected || isSaving.segregate_existing_files}
          className="mt-2 sm:mt-0 p-2 px-4 border border-green-400 text-green-400 hover:bg-green-400 hover:text-black transition-colors duration-200 disabled:border-gray-600 disabled:text-gray-600 disabled:hover:bg-transparent w-full sm:w-auto"
        >
          {isSaving.segregate_existing_files ? "Updating..." : "Update"}
        </button>
      </div>
    </div>
  );
}
