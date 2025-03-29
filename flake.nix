{
  description = "C++ development environment with Xlib support";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };
        libPath = with pkgs;
        lib.makeLibraryPath [ libGL xorg.libX11 ];
      in
      {
        devShells.default = pkgs.mkShell {
          nativeBuildInputs = with pkgs; [
            meson
            ninja
            gcc
            gnumake

            # Needed to build raylib
            cmake
            xorg.libXrandr
            xorg.libXinerama
            xorg.libXcursor
            xorg.libXi
            libGL
            xorg.libX11
          ];

          buildInputs = with pkgs; [
            clang-tools
            cpplint
            blender
          ];

          LD_LIBRARY_PATH = libPath;
        };
      }
    );
}
