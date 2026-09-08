{
  config,
  pkgs,
  lib,
  brainer,
  ...
}:
let
  cfg = config.services.brainerBot;
  settingsFormat = pkgs.formats.ini { };
  etcWorkingDirectory = "brainer";
  tasksFilename = "tasks.csv";
in
{
  options.services.brainerBot = {
    enable = lib.mkEnableOption "brainer bot service";
    tasksFile = lib.mkOption {
      type = lib.types.path;
      description = "";
    };
    resource = lib.mkOption {
      type = lib.types.nullOr lib.types.str;
      default = null;
    };
    room = lib.mkOption {
      type = lib.types.nullOr lib.types.str;
      default = null;
    };
    secretsFile = lib.mkOption {
      type = lib.types.pathWith { absolute = true; };
      description = "configuration format file with secrets";
    };
  };

  config = lib.mkIf cfg.enable {
    environment.etc = {
      "${etcWorkingDirectory + "/${tasksFilename}"}".source = cfg.tasksFile;
    };
    users.groups.brainer = { };
    users.users.brainer = {
      isSystemUser = true;
      group = "brainer";
    };
    systemd.services.brainer-bot =
      let
        configFile = pkgs.writeText "brainer_bot.conf" (
          lib.strings.concatLines (
            [
              "tasks_file=${tasksFilename}"
            ]
            ++ (lib.optional (!isNull cfg.resource) ''
              resource=${cfg.resource}
            '')
            ++ (lib.optional (!isNull cfg.room) ''
              room=${cfg.room}
            '')
          )
        );
      in
      {
        description = "brainer bot";
        wantedBy = [ "default.target" ]; # TODO: network
        serviceConfig.Type = "simple";
        serviceConfig.ExecStart = "${brainer}/bin/brainer_bot --config ${configFile} --config ${cfg.secretsFile}";
        serviceConfig.Restart = "always";
        serviceConfig.RestartSec = 5;
        serviceConfig.User = "${config.users.users.brainer.name}";
        serviceConfig.WorkingDirectory = "${"/etc/" + etcWorkingDirectory}";
        serviceConfig.PrivateTmp = "yes";
        serviceConfig.ProtectSystem = "full";
        serviceConfig.ProtectHome = "yes";
        serviceConfig.PrivateDevices = "yes";
        # TODO: security settings
      };
  };
}
