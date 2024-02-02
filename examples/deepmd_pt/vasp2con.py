import argparse
from ase.io import read, write

def convert_vasp_to_con(vasp_file, con_file):
    # 读取 .vasp 文件
    atoms = read(vasp_file)

    # 转换并保存为 .con 格式
    write(con_file, atoms)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Convert .vasp file to .con format.")
    parser.add_argument("vasp_file", type=str, help="Path to the .vasp file")
    parser.add_argument("con_file", type=str, help="Path for the output .con file")

    args = parser.parse_args()

    convert_vasp_to_con(args.vasp_file, args.con_file)

