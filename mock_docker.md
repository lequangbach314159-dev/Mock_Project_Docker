# README – Mock Python Version Check Project

## Giới thiệu

Dự án này mô phỏng một utility nhỏ cho môi trường OpenWRT/Linux embedded.
Ứng dụng viết bằng C để kiểm tra xem Python 3.9 đã được cài đặt hay chưa thông qua lệnh hệ thống.

Toàn bộ dự án được build và chạy trong Docker container để tạo môi trường độc lập, dễ triển khai và dễ kiểm thử.

---

## Thành phần chính

Dự án gồm các file sau:

- `check_python.c`
  - Source code C kiểm tra Python 3.9.
- `Dockerfile`
  - Tạo môi trường Docker, cài đặt dependency và build ứng dụng.
- `Makefile`
  - Hỗ trợ các lệnh build, chạy, clean và package.
- `check-python_1.0_mock.ipk`
  - File mock package OpenWRT `.ipk`.

---

## Mục đích

Ứng dụng thực hiện các bước sau:

1. Gọi lệnh kiểm tra Python 3.9:

   ```bash
   python3.9 --version
   ```

2. Nếu Python 3.9 tồn tại:

   - Hiển thị phiên bản Python.
   - Ghi log vào file `/tmp/python_ver.log`.

3. Nếu Python 3.9 không tồn tại:

   - Hiển thị `Error: Python 3.9 not found`.
   - Trả về mã lỗi khác 0.

---

## Quy trình hoàn chỉnh

### 1. Build và chạy Docker image

```bash
sudo docker build -t check-python-mock .
sudo docker run --rm check-python-mock
```

Kết quả đúng theo yêu cầu khi không có Python 3.9:

```text
Error: Python 3.9 not found
```

### 2. Tạo package mock `.ipk`

```bash
make package
ls
```

Nếu thấy file sau trong thư mục:

- `check-python_1.0_mock.ipk`

thì bước package đã thành công.

### 3. Git commit và tag

```bash
git add .
git commit -m "Add mock Python 3.9 version check utility"
git tag v1.0-python-check
git log --oneline --decorate
```

Nếu Git chưa biết tên/email và commit bị lỗi, cấu hình trước bằng:

```bash
git config --global user.email "bachlq4@example.com"
git config --global user.name "Bach LQ4"
```

Sau đó thực hiện lại commit, tag và kiểm tra log:

```bash
git commit -m "Add mock Python 3.9 version check utility"
git tag v1.0-python-check
git log --oneline --decorate
```

Bước cuối cùng là xác nhận trong `git log --oneline --decorate` có dòng commit kèm tag `v1.0-python-check`.

---

## Trạng thái hiện tại

Hiện tại dự án đã có đủ:

- `Dockerfile`
- `Makefile`
- Ứng dụng C kiểm tra Python
- File mock package `check-python_1.0_mock.ipk`
- Git branch
- Git commit
- Git tag `v1.0-python-check`

## Ảnh chụp màn hình

### Docker run và package

![Docker run và package](docker_run_and_ipk_package.png)

### Git tag và export file

![Git tag và export file](git_tag_and_export_ipk.png)

## Các file quan trọng

Các file chính trong dự án:

- `Dockerfile`
- `Makefile`
- `check_python.c`
- `check_python`
- `check-python_1.0_mock.ipk`
- `mock_docker.md`
- `docker_run_and_ipk_package.png`
- `git_tag_and_export_ipk.png`

---

## Nội dung mã nguồn và cấu hình

### `check_python.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int ret = system("python3.9 --version > /tmp/python_ver.log 2>&1");

    if (ret == -1) {
        printf("Error: cannot execute command\n");
        return 1;
    }

    if (WIFEXITED(ret) && WEXITSTATUS(ret) == 0) {
        FILE *fp = fopen("/tmp/python_ver.log", "r");
        char buf[256];

        if (fp && fgets(buf, sizeof(buf), fp)) {
            printf("Detected Python Version: %s", buf);
            fclose(fp);
        }

        return 0;
    } else {
        printf("Error: Python 3.9 not found\n");
        return 1;
    }
}
```

### `Makefile`

```make
APP=check_python

build:
	gcc check_python.c -o $(APP)

run: build
	./$(APP)

clean:
	rm -f $(APP)
	rm -f *.ipk
	rm -f /tmp/python_ver.log

package: build
	mkdir -p package/usr/bin
	cp $(APP) package/usr/bin/
	tar -czf check-python_1.0_mock.ipk package
```

### `Dockerfile`

```dockerfile
FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y \
    gcc \
    make \
    python3 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN make build

CMD ["./check_python"]
```

---

## Kết luận

Dự án đã hoàn thành các yêu cầu trong đề bài:

- Build và chạy chương trình trong Docker container.
- Kiểm tra kết quả khi Python 3.9 không có.
- Tạo package mock `.ipk`.
- Quản lý source bằng Git, commit và gắn tag.

Phần mở rộng tiếp theo có thể là tạo package OpenWRT thật hoặc cập nhật Dockerfile/Makefile để đóng gói cho môi trường embedded Linux.
