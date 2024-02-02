import argparse
from ase.io import read, write

def convert_con_to_vasp(con_file, vasp_file):
    # 读取 .con 文件
    atoms = read(con_file)

    # 转换并保存为 .vasp 格式
    write(vasp_file, atoms)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Convert .con file to .vasp format.")
    parser.add_argument("con_file", type=str, help="Path to the .con file")
    parser.add_argument("vasp_file", type=str, help="Path for the output .vasp file")

    args = parser.parse_args()

    convert_con_to_vasp(args.con_file, args.vasp_file)

