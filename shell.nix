
{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    pkg-config
    zig
    bear
    clang-tools
    compiledb
  ] ++ (if stdenv.isDarwin then [
    ] else [
  ]);

  shellHook = if pkgs.stdenv.isDarwin then ''
    export VK_ICD_FILENAMES=${pkgs.moltenvk}/share/vulkan/icd.d/MoltenVK_icd.json
  '' else "";
}
