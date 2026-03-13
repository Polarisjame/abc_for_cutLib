def compare_logs(file1_path, file2_path):
    with open(file1_path, 'r', encoding='utf-8') as f1, open(file2_path, 'r', encoding='utf-8') as f2:
        lines1 = f1.readlines()
        lines2 = f2.readlines()

    max_len = max(len(lines1), len(lines2))
    differences = []

    for i in range(max_len):
        line1 = lines1[i].strip() if i < len(lines1) else "<文件1缺少此行>"
        line2 = lines2[i].strip() if i < len(lines2) else "<文件2缺少此行>"

        if line1 != line2:
            differences.append((i+1, line1, line2))

    if not differences:
        print("✅ 两个 log 文件完全一致。")
    else:
        print(f"⚠️ 发现 {len(differences)} 处差异：")
        for lineno, l1, l2 in differences[:5]:
            print(f"\n第 {lineno} 行不同：")
            print(f"文件1: {l1}")
            print(f"文件2: {l2}")

# 使用示例
compare_logs("./RwrStrucutOri.log", "./RwrStrucutUpdated.log")