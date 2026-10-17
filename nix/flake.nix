{
  description = "Mandelbrot renderer env for C++";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        devShells.default = pkgs.mkShell {
          packages = with pkgs; [
            cmake
            ninja
            pkg-config
            SDL2
            hyperfine
          ] ++ lib.optionals stdenv.isLinux [
            gdb
            valgrind
          ];
        };

        packages.default = pkgs.stdenv.mkDerivation {
          pname = "fractal";
          version = "0.1.0";
          src = ./.;
          nativeBuildInputs = [ pkgs.cmake pkgs.ninja pkgs.pkg-config ];
          buildInputs = [ pkgs.SDL2 ];
        };
      });
}
