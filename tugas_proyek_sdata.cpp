#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <string>
#include <vector>
#include <ctime>


using namespace std;

struct KTP {
    string nomorKTP;
    string nama;
    string alamat;
    string tanggalLahir;

    void input() {
        cout << "Masukkan Nomor KTP: ";
        cin >> nomorKTP;
        cin.ignore();
        cout << "Masukkan Nama: ";
        getline(cin, nama);
        cout << "Masukkan Alamat: ";
        getline(cin, alamat);
        cout << "Masukkan Tanggal Lahir (YYYY-MM-DD): ";
        cin >> tanggalLahir;
    }

    void display() const {
        cout << "Nomor KTP      : " << nomorKTP << endl;
        cout << "Nama           : " << nama << endl;
        cout << "Alamat         : " << alamat << endl;
        cout << "Tanggal Lahir  : " << tanggalLahir << endl;
    }

    void saveToFile(ofstream &file) const {
        file << nomorKTP << "," << nama << "," << alamat << "," << tanggalLahir << endl;
    }
};

struct Item {
    string kode;
    string namaBarang;
    string tanggalMasuk;

    void input() {
        cout << "Masukkan Kode Item      : ";
        cin >> kode;
        cin.ignore();
        cout << "Masukkan Nama Barang    : ";
        getline(cin, namaBarang);
        cout << "Masukkan Tanggal Masuk  : ";
        cin >> tanggalMasuk;
    }

    void display() const {
        cout << "Kode Item      : " << kode << endl;
        cout << "Nama Barang    : " << namaBarang << endl;
        cout << "Tanggal Masuk  : " << tanggalMasuk << endl;
    }

    void saveToFile(ofstream &file) const {
        file <<"==============================\n"<<"kode barang : "<< kode
		 << "\nnama barang : " << namaBarang << "\ntanggal masuk :" << tanggalMasuk ;
		file <<"\n==============================\n";
    }
};

struct SoldItem {
    string kode;
    string namaBarang;
    string tanggalTerjual;

    void display() const {
        cout << "Kode Item      : " << kode << endl;
        cout << "Nama Barang    : " << namaBarang << endl;
        cout << "Tanggal Terjual: " << tanggalTerjual << endl;
    }
};

void adminSignIn(const string& nomorKTP) {
    ifstream file("data_ktp.txt");
    if (!file) {
        cerr << "Gagal membuka file data KTP.\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.find(nomorKTP) != string::npos) {
            cout << "Login berhasil!\n";
            return;
        }
    }
    cout << "Login gagal. Nomor KTP tidak ditemukan.\n";
    file.close();
}

void manageInventory(stack<Item> &lifo, queue<Item> &fifo) {
    Item item;
    item.input();
    lifo.push(item);
    fifo.push(item);
    ofstream file("data_barang.txt", ios::app);
    if (!file) {
        cerr << "Gagal membuka file data barang.\n";
        return;
    }
    item.saveToFile(file);
    file.close();
    cout << "Item berhasil ditambahkan ke gudang.\n";
}

void sellItem(stack<Item> &lifo, queue<Item> &fifo, vector<SoldItem> &soldItems, bool useLIFO) {
    if (useLIFO) {
        if (lifo.empty()) {
            cout << "Gudang kosong, tidak ada item untuk dijual.\n";
            return;
        }
        Item item = lifo.top();
        lifo.pop();
        SoldItem soldItem = {item.kode, item.namaBarang, "2025-01-15"}; // Contoh tanggal terjual
        soldItems.push_back(soldItem);
        cout << "Menjual item: \n";
        item.display();
    } else {
        if (fifo.empty()) {
            cout << "Gudang kosong, tidak ada item untuk dijual.\n";
            return;
        }
        Item item = fifo.front();
        fifo.pop();
        SoldItem soldItem = {item.kode, item.namaBarang, "2025-01-15"}; // Contoh tanggal terjual
        soldItems.push_back(soldItem);
        cout << "Menjual item: \n";
        item.display();
    }
}

void displayItems(const stack<Item> &lifo, const queue<Item> &fifo) {
    cout << "\nBarang di gudang (LIFO):\n";
    cout << "----------------------------------------\n";
    stack<Item> tempLIFO = lifo;
    while (!tempLIFO.empty()) {
        tempLIFO.top().display();
        cout << "----------------------------------------\n";
        tempLIFO.pop();
    }
cout<<"\n\n\n\n";
    cout << "\nBarang di gudang (FIFO):\n";
    cout << "----------------------------------------\n";
    queue<Item> tempFIFO = fifo;
    while (!tempFIFO.empty()) {
        tempFIFO.front().display();
        cout << "----------------------------------------\n";
        tempFIFO.pop();
    }
}

void displaySoldItems(const vector<SoldItem> &soldItems) {
    if (soldItems.empty()) {
        cout << "Tidak ada barang yang terjual.\n";
        return;
    }
    cout << "\nBarang yang sudah terjual:\n";
    cout << "----------------------------------------\n";
    for (const auto &item : soldItems) {
        item.display();
        cout << "----------------------------------------\n";
    }
cout<<endl;
cout<<endl;
}

void deleteItem(stack<Item> &lifo, queue<Item> &fifo, const string &kodeItem) {
    stack<Item> tempLIFO;
    queue<Item> tempFIFO;
    bool found = false;

    // Filter LIFO stack
    while (!lifo.empty()) {
        if (lifo.top().kode == kodeItem) {
            found = true;
        } else {
            tempLIFO.push(lifo.top());
        }
        lifo.pop();
    }

    // Restore remaining items
    while (!tempLIFO.empty()) {
        lifo.push(tempLIFO.top());
        tempLIFO.pop();
    }

    // Filter FIFO queue
    while (!fifo.empty()) {
        if (fifo.front().kode == kodeItem) {
            found = true;
        } else {
            tempFIFO.push(fifo.front());
        }
        fifo.pop();
    }

    // Restore remaining items
    while (!tempFIFO.empty()) {
        fifo.push(tempFIFO.front());
        tempFIFO.pop();
    }

    if (found) {
        cout << "Item dengan kode " << kodeItem << " berhasil dihapus.\n";
    } else {
        cout << "Item dengan kode " << kodeItem << " tidak ditemukan.\n";
    }
}

void loadItemsFromFile(stack<Item> &lifo, queue<Item> &fifo) {
    ifstream file("data_barang.txt");
    if (!file) {
        cerr << "Gagal membuka file data barang.\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        string kode, namaBarang, tanggalMasuk;
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);

        if (pos1 != string::npos && pos2 != string::npos) {
            kode = line.substr(0, pos1);
            namaBarang = line.substr(pos1 + 1, pos2 - pos1 - 1);
            tanggalMasuk = line.substr(pos2 + 1);

            Item item = {kode, namaBarang, tanggalMasuk};
            lifo.push(item);
            fifo.push(item);
        }
    }
    file.close();
}

int main() {
    stack<Item> lifo;
    queue<Item> fifo;
    vector<SoldItem> soldItems;

    const string dataBarangFile = "data_barang.txt";

    loadItemsFromFile(lifo, fifo);

    int choice;
    do {
        cout << "\nMenu:\n";
        cout << "1. Tambahkan Data KTP Baru\n";
        cout << "2. Admin Sign In\n";
        cout << "3. Entri Item Barang\n";
        cout << "4. Jual Item Barang (LIFO)\n";
        cout << "5. Jual Item Barang (FIFO)\n";
        cout << "6. Cetak Daftar Barang\n";
        cout << "7. Tampilkan Barang Terjual\n";
        cout << "8. Hapus Barang\n";
        cout << "9. Keluar\n";
        cout << "Pilih: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                ofstream file("data_ktp.txt", ios::app);
                if (!file) {
                    cout << "Gagal membuka file data KTP untuk ditulis.\n";
                    break;
                }
                KTP ktp;
                ktp.input();
                ktp.saveToFile(file);
                file.close();
                cout << "Data KTP berhasil ditambahkan.\n";
                break;
            }
            case 2: {
                string nomorKTP;
                cout << "Masukkan Nomor KTP untuk Login: ";
                cin >> nomorKTP;
                adminSignIn(nomorKTP);
                break;
            }
            case 3:
                manageInventory(lifo, fifo);
                break;
            case 4:
                sellItem(lifo, fifo, soldItems, true);
                break;
            case 5:
                sellItem(lifo, fifo, soldItems, false);
                break;
            case 6:
                displayItems(lifo, fifo);
                break;
            case 7:
                displaySoldItems(soldItems);
                break;
            case 8: {
                string kodeItem;
                cout << "Masukkan kode item yang ingin dihapus: ";
                cin >> kodeItem;
                deleteItem(lifo, fifo, kodeItem);
                break;
            }
            case 9:
                cout << "Keluar dari program.\n";
                break;
            default:
                cout << "Pilihan tidak valid.\n";
        }
    } while (choice != 9);

    return 0;
}

