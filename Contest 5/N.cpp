#include <ios>
#include <vector>
#include <iostream>

struct Table {
    size_t table_size;
    size_t    symbolic_link;

    explicit Table()                   : table_size(0)          , symbolic_link(-1) {}
    explicit Table(size_t _table_size) : table_size(_table_size), symbolic_link(-1) {}
};

size_t maxi = 0;

class DB {
private:
    std::vector<Table> tables;
    std::vector<size_t>   ranks;
    std::vector<size_t>   realLinks;

public:
    explicit DB() = default;

    void addTable(size_t _table_size) {
        tables   .push_back(Table(_table_size));
        ranks    .push_back(0);
        realLinks.push_back(tables.size() - 1);

        maxi = std::max(maxi, _table_size);
    }

    size_t getRealLink(size_t _table_num) {
        if (_table_num == realLinks[_table_num]) return _table_num;
        return realLinks[_table_num] = getRealLink(realLinks[_table_num]);
    }

    void uniteTables(size_t _dest, size_t _source) {
        size_t dest = getRealLink(_dest), source = getRealLink(_source);
        if (dest == source) return;

        if (ranks[dest] <  ranks[source]) std::swap(dest, source);
        if (ranks[dest] == ranks[source]) ranks[dest]++;

        realLinks[source] = dest;
        tables[dest].table_size += tables[source].table_size;

        maxi = std::max(maxi, tables[dest].table_size);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0); std::cout.tie(0);

    size_t n = 0, m = 0;
    std::cin >> n >> m;

    DB mainDB = DB();

    for (size_t i = 0; i < n; i++) {
        size_t size = 0;
        std::cin >> size;

        mainDB.addTable(size);
    }

    for (size_t i = 0; i < m; i++) {
        size_t dest = 0, source = 0;
        std::cin >> dest >> source;

        mainDB.uniteTables(--dest, --source);
        std::cout << maxi << '\n';
    }

    return 0;
}