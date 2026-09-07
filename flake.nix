{
  description = "brainer";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs";
    flake-utils.url = "github:numtide/flake-utils";
    treefmt-nix = {
      url = "github:numtide/treefmt-nix";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      treefmt-nix,
      ...
    }:
    flake-utils.lib.eachSystem nixpkgs.lib.systems.flakeExposed (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        xlog = pkgs.callPackage ./nix/dep/xlog.nix { };
        makePackage = args: pkgs.callPackage ./nix/build.nix ({ inherit xlog; } // args);
      in
      rec {
        packages = {
          default = makePackage { };
        };
        devShells = {
          default = pkgs.callPackage ./nix/shell.nix { brainer = makePackage { withGui = true; }; };
        };
      }
    );
}
