{
  stdenv,
  lib,
  cmake,
  ninja,
  pkg-config,
  gloox,
  xlog,
  libidn,
  zlib,
  openssl,
  pugixml,
  cppunit,
  cli11,
  qt6,
  withBot ? true,
  withGui ? false,
  ...
}:
stdenv.mkDerivation {
  name = "brainer";
  src = ./..;
  nativeBuildInputs = [
    cmake
    ninja
    pkg-config
  ];
  buildInputs = [
    cppunit
    xlog
    libidn
    zlib
    openssl
    pugixml
  ]
  ++ lib.optionals withBot [
    gloox
    cli11
  ]
  ++ lib.optional withGui qt6.qtbase;
  cmakeFlags = lib.optional withBot "-DENABLE_BOT=on" ++ lib.optional withGui "-DENABLE_GUI=on";
}
