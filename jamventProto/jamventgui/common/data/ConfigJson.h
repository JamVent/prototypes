#ifndef CONFIGJSON_H
#define CONFIGJSON_H

class ConfigJson
{
  public:
    static constexpr size_t UlIndex = 0;    // Upper left graph
    static constexpr size_t UrIndex = 1;    // Upper right graph
    static constexpr size_t LlIndex = 2;    // Lower left graph
    static constexpr size_t LrIndex = 3;    // Lower right grapn
    static constexpr size_t GraphCount = 4; // 4 graphs

    static constexpr const char *ConfigModeSwSim = "sw-sim";
    static constexpr const char *ConfigModeHw = "hw";

    static constexpr const char *CommModeSwSim = "i2c";
    static constexpr const char *CommModeHw = "serial";

    struct GraphConfigData
    {
      bool isSet = false;  // true when values are set. Empty otherwise.
      double xAxisMin = 0.0;
      double xAxisMax = 0.0;
      double yAxisMin = 0.0;
      double yAxisMax = 0.0;
      QString yAxisLabel = "";
    };

    struct ConfigData
    {
        QString configMode = ""; ///< Allowed values: "sw-sim", "hw"
        QString commMode = "";   ///< Allowed values: "i2c", "serial". Not used
        QString debugMode = "";  ///< Allowed values: "debug". Not used
        int graphTickCount = 0;
        qreal pollingTimeout_ms = 0.0;
        GraphConfigData graphs[GraphCount];
    };

    static ConfigJson& Instance() {
        static ConfigJson s_configJson;
        return s_configJson;
    }

    // Expose constant config data
    const ConfigData& getConfigData();

    // Open and parse the config file.
    void parseConfigFile();

  private:
    explicit ConfigJson();
    ~ConfigJson();
    ConfigData m_data;
};
#endif /* CONFIGJSON_H */
