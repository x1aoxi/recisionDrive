# LVGL v8.3.11 移植完成 - Keil配置指南

## ✅ 已完成的工作

1. 创建了配置文件: `lv_conf.h` (工程根目录)
2. 创建了移植层文件:
   - `User/App/lv_port_disp.h/c` (显示驱动)
   - `User/App/lvgl_app.h/c` (应用层)
3. 修改了 `main.c` 添加LVGL初始化和任务调用

---

## 🔧 Keil工程配置步骤

### 步骤1: 添加LVGL源文件

1. 在Keil Project窗口,右键项目 → **Manage Project Items**
2. 创建新组 `LVGL`
3. 点击 **Add Files...**
4. 浏览到 `E:\校电赛\lvgl\src\`
5. **重要**: 使用通配符添加所有子文件夹的 `.c` 文件:
   - 进入 `src\core` 全选 `*.c` 文件 → Add
   - 进入 `src\draw` 全选 `*.c` 文件 → Add
   - 进入 `src\font` 全选 `lv_font.c`, `lv_font_fmt_txt.c`, `lv_font_montserrat_12.c` → Add
   - 进入 `src\hal` 全选 `*.c` 文件 → Add
   - 进入 `src\misc` 全选 `*.c` 文件 → Add
   - 进入 `src\widgets` 全选 `*.c` 文件 → Add
6. 添加移植层文件:
   - `User\App\lv_port_disp.c`
   - `User\App\lvgl_app.c`

---

### 步骤2: 添加头文件路径

1. 右键项目 → **Options for Target...**
2. **C/C++** 标签页 → **Include Paths** → 点击 **...**
3. 添加以下3个路径:
   ```
   ..
   ..\校电赛\lvgl
   ..\校电赛\lvgl\src
   ```

**说明**:
- `..` - 工程根目录(包含lv_conf.h)
- `..\校电赛\lvgl` - LVGL库根目录
- `..\校电赛\lvgl\src` - LVGL源码目录

---

### 步骤3: 编译

点击编译按钮,应该能成功编译!

---

## 📊 文件清单

### 需要添加的源文件 (约60-70个)

```
LVGL组:
  E:\校电赛\lvgl\src\core\*.c        (15个)
  E:\校电赛\lvgl\src\draw\*.c        (直接在draw目录的,不含子文件夹)
  E:\校电赛\lvgl\src\font\lv_font.c  (只3个字体文件)
  E:\校电赛\lvgl\src\hal\*.c         (3个)
  E:\校电赛\lvgl\src\misc\*.c        (约20个)
  E:\校电赛\lvgl\src\widgets\*.c     (约20个)

  User\App\lv_port_disp.c
  User\App\lvgl_app.c
```

---

## ⚠️ 注意事项

1. **不要添加** `src\extra` 目录的文件
2. **不要添加** `src\draw` 子文件夹(nxp, sdl, sw等)
3. **只添加** font目录中的3个文件,不要全选
4. 如果编译出错,检查头文件路径是否正确

---

## 🎯 预期结果

编译成功后烧录到板子,OLED应该显示 "LVGL OK!" 文字,居中显示。

---

## 🐛 常见问题

### Q: 找不到 lvgl.h
**A**: 检查头文件路径是否添加了 `..\校电赛\lvgl`

### Q: 编译时间太长
**A**: 首次编译约2-3分钟,后续增量编译很快

### Q: 提示缺少某个.c文件
**A**: 根据错误提示,到对应目录添加缺失的文件
