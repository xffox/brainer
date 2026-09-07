{
  stdenv,
  fetchgit,
  cmake,
  ...
}:
stdenv.mkDerivation {
  name = "xlog";
  src = fetchgit {
    url = "https://github.com/xffox/xlog";
    rev = "v0.1.1";
    sha256 = "sha256-eXSCbUn9k/sbsgm2gWzZuNFU5RkoJVVb9BC1cvCWa5s=";
  };

  nativeBuildInputs = [
    cmake
  ];

  cmakeFlags = [
    "-DDISABLE_SAMPLE=on"
  ];
}
