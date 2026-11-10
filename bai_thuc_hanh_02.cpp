#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <ctime>
using namespace std;

class phuong_tien {
private:
    int ma_xe;
    string loai_xe;
    int gio_vao;
    int tg_do;
    double phi_do;

public:
    phuong_tien() {
        ma_xe = 0;
        loai_xe = "";
        gio_vao = 0;
        tg_do = 0;
        phi_do = 0;
    }

    phuong_tien(int ma, string loai, int gio, int tg) {
        ma_xe = ma;
        loai_xe = loai;
        gio_vao = gio;
        tg_do = tg;
        phi_do = tinh_phi();
    }

    int lay_ma_xe() const {
        return ma_xe;
    }

    string lay_loai_xe() const {
        return loai_xe;
    }

    double tinh_phi() const {
        double gia;
        if (loai_xe == "Car") gia = 6.0;
        else if (loai_xe == "Motorcycle") gia = 4.0;
        else gia = 2.0;

        bool cao_diem = (gio_vao >= 6 && gio_vao <= 9) ||
                        (gio_vao >= 15 && gio_vao <= 18);

        double tong = 0;
        for (int i = 0; i < tg_do; i++) {
            if (i < 2) {
                if (cao_diem) tong += gia * 1.30;
                else tong += gia * 0.95;
            } else {
                tong += gia * 0.90;
            }
        }
        return tong;
    }

    void hien_thi() const {
        cout << "  ID: " << ma_xe << " | " << loai_xe
             << " | " << gio_vao << "h | " << tg_do << "h"
             << " | Phi: $" << fixed << setprecision(2) << tinh_phi() << "\n";
    }
};

class bai_xe {
private:
    static const int MAX_VEHICLES = 3;
    int ma_bai;
    char kich_thuoc;
    phuong_tien ds_xe[MAX_VEHICLES];
    int so_xe;

public:
    bai_xe() {
        ma_bai = 0;
        kich_thuoc = 'S';
        so_xe = 0;
    }

    bai_xe(int ma, char kt) {
        ma_bai = ma;
        kich_thuoc = kt;
        so_xe = 0;
    }

    int lay_ma() const {
        return ma_bai;
    }

    int suc_chua() const {
        if (kich_thuoc == 'S') return 1;
        if (kich_thuoc == 'M') return 2;
        return 3;
    }

    bool day() const {
        return so_xe >= suc_chua();
    }

    bool them(phuong_tien v) {
        if (day()) return false;
        ds_xe[so_xe] = v;
        so_xe++;
        return true;
    }

    int tim_xe(int ma_xe) const {
        for (int i = 0; i < so_xe; i++) {
            if (ds_xe[i].lay_ma_xe() == ma_xe) return i;
        }
        return -1;
    }

    bool xoa(int ma_xe) {
        int vt = tim_xe(ma_xe);
        if (vt == -1) return false;

        for (int i = vt; i < so_xe - 1; i++) {
            ds_xe[i] = ds_xe[i + 1];
        }
        so_xe--;
        return true;
    }

    void hien_thi() const {
        cout << "Bai " << ma_bai << " [" << kich_thuoc << "] "
             << so_xe << "/" << suc_chua() << " xe\n";
        for (int i = 0; i < so_xe; i++) {
            ds_xe[i].hien_thi();
        }
    }

    bool co_loai(string loai, int& dem, double& tong) const {
        dem = 0;
        tong = 0;
        for (int i = 0; i < so_xe; i++) {
            if (ds_xe[i].lay_loai_xe() == loai) {
                dem++;
                tong += ds_xe[i].tinh_phi();
            }
        }
        return dem > 0;
    }
};

class SystemInterface {
private:
    static const int MAX_LOTS = 10;
    bai_xe ds_bai[MAX_LOTS];
    int so_bai;

    int nhap_so(string thong_bao, int min, int max) {
        int x;
        while (true) {
            cout << thong_bao;
            cin >> x;
            if (cin.fail() || x < min || x > max) {
                cout << "Du lieu khong hop le. Vui long nhap lai!\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return x;
            }
        }
    }

    char nhap_kich_thuoc() {
        char kt;
        while (true) {
            cout << "Kich thuoc (S/M/L): ";
            cin >> kt;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (kt == 'S' || kt == 's') return 'S';
            if (kt == 'M' || kt == 'm') return 'M';
            if (kt == 'L' || kt == 'l') return 'L';
            cout << "Kich thuoc khong hop le. Vui long nhap lai!\n";
        }
    }

    string nhap_loai_xe() {
        string loai;
        while (true) {
            cout << "Loai (Car/Motorcycle/Bicycle): ";
            cin >> loai;
            if (loai == "Car" || loai == "Motorcycle" || loai == "Bicycle") return loai;
            cout << "Loai xe khong hop le. Vui long nhap lai!\n";
        }
    }

    int tim_bai(int ma_bai) const {
        for (int i = 0; i < so_bai; i++) {
            if (ds_bai[i].lay_ma() == ma_bai) return i;
        }
        return -1;
    }

    bool isParkIDExists(int ma_bai) const {
        return tim_bai(ma_bai) != -1;
    }

    bool isVehicleIDExists(int ma_xe) const {
        for (int i = 0; i < so_bai; i++) {
            if (ds_bai[i].tim_xe(ma_xe) != -1) return true;
        }
        return false;
    }

    int generateRandomID() const {
        int ma;
        do {
            ma = 10000 + rand() % 90000;
        } while (isParkIDExists(ma) || isVehicleIDExists(ma));
        return ma;
    }

public:
    SystemInterface() {
        so_bai = 0;
        srand((unsigned int)time(0));
    }

    void runMenu() {
        int chon;

        do {
            cout << "\n===== QUAN LY BAI DO XE =====\n";
            cout << "1. Tao bai do\n";
            cout << "2. Them xe\n";
            cout << "3. Xoa bai do\n";
            cout << "4. Lay xe ra\n";
            cout << "5. Hien thi tat ca bai do\n";
            cout << "6. Hien thi xe theo bai\n";
            cout << "7. Liet ke xe theo loai\n";
            cout << "8. Bao cao phi trung binh\n";
            cout << "9. Thoat\n";
            chon = nhap_so("Chon: ", 1, 9);

            switch (chon) {
                case 1: {
                    if (so_bai >= MAX_LOTS) {
                        cout << "He thong da day bai do!\n";
                        break;
                    }

                    int ma = nhap_so("Ma bai: ", 1, 1000000000);
                    if (isParkIDExists(ma)) {
                        cout << "Ma bai da ton tai. Tu dong tao ma moi.\n";
                        ma = generateRandomID();
                    }

                    char kt = nhap_kich_thuoc();
                    ds_bai[so_bai] = bai_xe(ma, kt);
                    so_bai++;
                    cout << "Da tao bai " << ma << "\n";
                    break;
                }

                case 2: {
                    if (so_bai == 0) {
                        cout << "Chua co bai do nao. Hay tao bai do truoc!\n";
                        break;
                    }

                    int ma = nhap_so("Ma xe: ", 1, 1000000000);
                    if (isVehicleIDExists(ma)) {
                        cout << "Ma xe da ton tai. Tu dong tao ma moi.\n";
                        ma = generateRandomID();
                    }

                    string loai = nhap_loai_xe();
                    int gio = nhap_so("Gio vao (0-23): ", 0, 23);
                    int tg = nhap_so("Thoi luong (1-23): ", 1, 23);
                    phuong_tien xe(ma, loai, gio, tg);

                    bool da_them = false;
                    for (int i = 0; i < so_bai; i++) {
                        if (!ds_bai[i].day()) {
                            ds_bai[i].them(xe);
                            da_them = true;
                            break;
                        }
                    }

                    if (da_them) {
                        cout << "Da them xe " << ma << " | Phi: $"
                             << fixed << setprecision(2) << xe.tinh_phi() << "\n";
                    } else {
                        cout << "Tat ca bai do da day!\n";
                    }
                    break;
                }

                case 3: {
                    int ma = nhap_so("Ma bai can xoa: ", 1, 1000000000);
                    int vt = tim_bai(ma);
                    if (vt == -1) {
                        cout << "Khong tim thay bai do!\n";
                    } else {
                        for (int i = vt; i < so_bai - 1; i++) {
                            ds_bai[i] = ds_bai[i + 1];
                        }
                        so_bai--;
                        cout << "Da xoa bai " << ma << "\n";
                    }
                    break;
                }

                case 4: {
                    int ma = nhap_so("Ma xe can lay ra: ", 1, 1000000000);
                    bool da_xoa = false;
                    for (int i = 0; i < so_bai; i++) {
                        if (ds_bai[i].xoa(ma)) {
                            da_xoa = true;
                            break;
                        }
                    }
                    if (da_xoa) cout << "Da lay xe ra\n";
                    else cout << "Khong tim thay xe\n";
                    break;
                }

                case 5: {
                    if (so_bai == 0) cout << "Chua co bai do nao!\n";
                    for (int i = 0; i < so_bai; i++) {
                        ds_bai[i].hien_thi();
                    }
                    break;
                }

                case 6: {
                    int ma = nhap_so("Ma bai: ", 1, 1000000000);
                    int vt = tim_bai(ma);
                    if (vt == -1) cout << "Khong tim thay bai do!\n";
                    else ds_bai[vt].hien_thi();
                    break;
                }

                case 7: {
                    string loai = nhap_loai_xe();
                    bool tim_thay = false;
                    for (int i = 0; i < so_bai; i++) {
                        int dem;
                        double tong;
                        if (ds_bai[i].co_loai(loai, dem, tong)) {
                            ds_bai[i].hien_thi();
                            tim_thay = true;
                        }
                    }
                    if (!tim_thay) cout << "Khong co xe thuoc loai nay!\n";
                    break;
                }

                case 8: {
                    string loai_arr[3] = {"Car", "Motorcycle", "Bicycle"};
                    for (int j = 0; j < 3; j++) {
                        int dem = 0;
                        double tong = 0;

                        for (int i = 0; i < so_bai; i++) {
                            int d;
                            double s;
                            ds_bai[i].co_loai(loai_arr[j], d, s);
                            dem += d;
                            tong += s;
                        }

                        cout << loai_arr[j] << ": ";
                        if (dem == 0) {
                            cout << "khong co xe\n";
                        } else {
                            cout << dem << " xe | TB: $" << fixed
                                 << setprecision(2) << tong / dem << "\n";
                        }
                    }
                    break;
                }

                case 9:
                    cout << "Da thoat chuong trinh!\n";
                    break;
            }
        } while (chon != 9);
    }
};

int main() {
    SystemInterface sys;
    sys.runMenu();
    return 0;
}
