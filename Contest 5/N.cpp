#include <ios>
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
    std::vector<int>   realLinks;

public:
    explicit DB() = default;

    void addTable(size_t _table_size) {
        tables   .push_back(Table(_table_size));
        realLinks.push_back(-1);
    }

    int getRealLink(size_t _table_num) {
        int realLink = realLinks[_table_num];
        return realLink == -1 ? _table_num : realLink;
    }

    void uniteTables(int _dest, int _source) {
        size_t dest = getRealLink(_dest), source = getRealLink(_source);
        if (dest == source) return;

        tables[dest]  .table_size += tables[source].table_size;

        realLinks[source]         = getRealLink(dest);
        tables[source].table_size = 0;

        maxi = std::max(maxi, tables[dest].table_size);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0); std::cout.tie(0);

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