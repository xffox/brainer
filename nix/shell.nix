{
  mkShell,
  brainer,
  clang-tools,
  nixfmt,
  ...
}:
mkShell {
  inputsFrom = [ brainer ];
  packages = [
    clang-tools
    nixfmt
  ];
  env = {
    CMAKE_EXPORT_COMPILE_COMMANDS = "on";
  };
}
