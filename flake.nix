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
    let
      makePackage =
        cur: args:
        cur.callPackage ./nix/build.nix ({ xlog = cur.callPackage ./nix/dep/xlog.nix { }; } // args);
    in
    flake-utils.lib.eachSystem nixpkgs.lib.systems.flakeExposed (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      rec {
        packages = {
          default = makePackage pkgs { };
        };
        devShells = {
          default = pkgs.callPackage ./nix/shell.nix { brainer = makePackage pkgs { withGui = true; }; };
        };
      }
    )
    // {
      nixosModules = {
        brainer =
          args@{
            pkgs,
            ...
          }:
          import ./nix/modules/brainer.nix (args // { brainer = makePackage pkgs { }; });
      };
    };
}
