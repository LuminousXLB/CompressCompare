#include "LZ78Coder.h"

void LZ78Encoder::encode()
{
    if (!ibs.is_open() || !obs.is_open()) {
        throw "File not Opened";
    }

#ifdef DEBUG
    DEBUG_HEADER;
    printf("\n");
#endif // DEBUG

    Bit buf;
    size_t next = 1;
    size_t len = 1, mcnt = 2;
    size_t prefix_state;

    Tree<Pair> tree(Pair { 0, 0 });

    TreeNode<Pair>* current = NULL;

    while (ibs.getb(buf)) {
        // Maintain the len
        if (next - 1 == mcnt) {
            mcnt *= 2;
            len++;
        }

        // Matching the prefix
        current = tree.getRoot();

#ifdef LZ78_ENCODER_DEBUG_READ
        DEBUG_HEADER;
#endif // LZ78_ENCODER_DEBUG_READ

        do {
#ifdef LZ78_ENCODER_DEBUG_READ
            printf("%d", buf);
#endif // LZ78_ENCODER_DEBUG_READ
            if (current->get(buf)) {
                current = current->get(buf);
            } else {
                break;
            }
        } while (ibs.getb(buf));

        prefix_state = current->getValue().state;

        if (!ibs.b_eof()) {
            // Expand the tree with unreconized symbol
            current->addChild(buf, { prefix_state, next++ });
        }

#ifdef LZ78_ENCODER_DEBUG_READ
        std::cout << std::endl;
#endif // LZ78_ENCODER_DEBUG_READ

        // Code it and write to outfile
        Bit* buffer = new Bit[len];

        for (size_t i = 0; i < len; i++) {
            buffer[len - i - 1] = prefix_state % 2;
            prefix_state >>= 1;
        }

#ifdef LZ78_ENCODER_DEBUG_WRITE
        DEBUG_HEADER;
        for (size_t i = 0; i < len; i++) {
            printf("%d", buffer[i]);
        }

        if (!ibs.b_eof()) {
            printf(" %d\n", buf);
        } else {
            printf("\n");
        }
#endif // LZ78_ENCODER_DEBUG_WRITE

        obs.puts(buffer, len);

        if (!ibs.b_eof()) {
            obs.putb(buf);
        }

        delete[] buffer;
        buffer = NULL;
    }

#ifdef LZ78_ENCODER_DEBUG_CNT
    DEBUG_HEADER;
    printf("%lld\n", obs.getCount());
#endif // LZ78_ENCODER_DEBUG_CNT

    obs.dumpLength();
}

SSIZE LZ78Decoder::partition_cnt()
{
    const SSIZE file_Len = ibs.extractLength();

    unsigned ord_pC = ordL2N[lb(file_Len)];

    SSIZE part_Cnt = (file_Len + (1ull << (ord_pC + 1)) - 2) / (ord_pC + 2);

    if (lb(part_Cnt) == ord_pC) {
        return part_Cnt;
    }

    ord_pC--;
    part_Cnt = (file_Len + (1ull << (ord_pC + 1)) - 2) / (ord_pC + 2);

    if (lb(part_Cnt) == ord_pC) {
        return part_Cnt;
    }

#ifdef DEBUG
    DEBUG_HEADER;
    printf("Partition Calculate Failed with L = %llu\n", file_Len);
#endif // DEBUG

    throw file_Len;

    return 0;
}

void LZ78Decoder::decode()
{
    if (!ibs.is_open() || !obs.is_open()) {
        throw "File not Opened";
    }

#ifdef DEBUG
    DEBUG_HEADER;
    printf("\n");
#endif // DEBUG

    std::vector<Code*> table;
    table.reserve(size_t(partition_cnt() + 1));
    table.push_back(nullptr);

    Bit* prefixbuf = nullptr;
    Bit tail;

    size_t len = 1, mcnt = 2;
    size_t prefix_state;

    while (ibs.gets(prefixbuf, len)) {
        // Extract the prefix state code
        prefix_state = prefixbuf[0];
        for (size_t i = 1; i < len; i++) {
            prefix_state = (prefix_state << 1) + prefixbuf[i];
        }

        // Fetch the tail
        ibs.getb(tail);

#ifdef LZ78_DECODER_DEBUG
        DEBUG_HEADER;
        printf("<%lu : %lu> ", table.size(), prefix_state);
#endif // LZ78_DECODER_DEBUG

        if (!ibs.b_eof()) {
            // if not eof
            Code* ptr = new Code();
            table.push_back(ptr);

            if (table[prefix_state] != nullptr) {
                // if prefix exists
                Code* prefix = table[prefix_state];

                ptr->pattern_len = prefix->pattern_len + 1;
                ptr->pattern = new Bit[ptr->pattern_len];
                memcpy_s(ptr->pattern, ptr->pattern_len, prefix->pattern, prefix->pattern_len);
                ptr->pattern[prefix->pattern_len] = tail;
            } else {
                // if prefix does not exist
                ptr->pattern_len = 1;
                ptr->pattern = new Bit[1];
                ptr->pattern[0] = tail;
            }

#ifdef LZ78_DECODER_DEBUG
            for (size_t i = 0; i < ptr->pattern_len; i++) {
                printf("%d", ptr->pattern[i]);
            }
            std::cout << std::endl;
#endif // LZ78_DECODER_DEBUG

            // write to the file
            obs.puts(ptr->pattern, ptr->pattern_len);
        } else if (table[prefix_state] != nullptr) {
            Code* prefix = table[prefix_state];

#ifdef LZ78_DECODER_DEBUG
            for (size_t i = 0; i < prefix->pattern_len; i++) {
                printf("%d", prefix->pattern[i]);
            }
            std::cout << std::endl;
#endif // LZ78_DECODER_DEBUG

            obs.puts(prefix->pattern, prefix->pattern_len);

            break;
        }

        // Maintain the len
        if (table.size() - 1 == mcnt) {
            mcnt *= 2;
            len++;
        }
    }

    size_t table_size = table.size();

    for (size_t i = 0; i < table_size; i++) {
        if (table[i]) {
            delete table[i];
        }
    }
}
