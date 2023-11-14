#include <vector>
#include <iostream>

struct Table {
    size_t table_size;
    int    symbolic_link;

    explicit Table()                   : table_size(0)          , symbolic_link(-1) {}
    explicit Table(size_t _table_size) : table_size(_table_size), symbolic_link(-1) {}
};

size_t maxi = 0;

class DB {
private:
    std::vector<Table> tables;

public:
    explicit DB() = default;

    void addTable(size_t _table_size) {
        tables.push_back(Table(_table_size));
    }

    int getRealLink(size_t _table_num) {
        int realLink = _table_num;
        int tSymLink = tables[_table_num].symbolic_link;

        while (tSymLink != -1) {
            realLink = tSymLink;
            tSymLink = tables[tSymLink].symbolic_link;
        }

        return realLink;
    }

    void uniteTables(int _dest, int _source) {
        size_t dest = getRealLink(_dest), source = getRealLink(_source);
        if (dest == source) return;

        tables[dest]  .table_size += tables[source].table_size;

        tables[source].symbolic_link = dest;
        tables[source].table_size    = 0;

        maxi = std::max(maxi, tables[dest].table_size);
    }
};

int main() {
    int n = 0, m = 0;
    std::cin >> n >> m;

    DB mainDB = DB();

    for (int i = 0; i < n; i++) {
        int size = 0;
        std::cin >> size;

        mainDB.addTable(size);
    }

    for (int i = 0; i < m; i++) {
        int dest = 0, source = 0;
        std::cin >> dest >> source;

        mainDB.uniteTables(--dest, --source);
        std::cout << maxi << '\n';
    }

    return 0;
}