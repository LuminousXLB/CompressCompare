#include "LZ78ByteCoder.h"

void LZ78ByteEncoder::encode()
{
#ifdef LZ78_BYTE_ENCODER_DEBUG
    DEBUG_HEADER;
    printf("\n");
#endif // LZ78_BYTE_ENCODER_DEBUG

    Byte buf;
    size_t next = 1;
    size_t len = 1, mcnt = 2;
    size_t prefix_state;

    Tree<Pair, 256> tree(Pair { 0, 0 });

    TreeNode<Pair, 256>* current = NULL;

    while (fread(&buf, sizeof(Byte), 1, ifp) > 0) {

        //while (buf = fgetc(ifp) && !feof(ifp)) {
        // Maintain the len
        if (next - 1 == mcnt) {
            mcnt *= 2;
            len++;
        }

        // Matching the prefix
        current = tree.getRoot();

#ifdef LZ78_BYTE_ENCODER_DEBUG_READ
        DEBUG_HEADER;
#endif // LZ78_BYTE_ENCODER_DEBUG_READ

        do {

#ifdef LZ78_BYTE_ENCODER_DEBUG_READ
            hexdump(&buf, 1);
            printf(" ");
#endif // LZ78_BYTE_ENCODER_DEBUG_READ

            if (current->get(buf)) {
                current = current->get(buf);
            } else {
                break;
            }
            //} while (buf = fgetc(ifp));
        } while (fread(&buf, sizeof(Byte), 1, ifp) > 0);

#ifdef LZ78_BYTE_ENCODER_DEBUG_READ
        cout << endl;
#endif // LZ78_BYTE_ENCODER_DEBUG_READ

        prefix_state = current->getValue().state;

#ifdef LZ78_BYTE_ENCODER_DEBUG_WRITE
        DEBUG_HEADER;
        printf("\t\t%d - %d\t", prefix_state, next);

#endif // LZ78_BYTE_ENCODER_DEBUG_WRITE

        if (!feof(ifp)) {
            // Expand the tree with unreconized symbol
            current->addChild(buf, { prefix_state, next++ });
        }

        // Code it and write to outfile
        Bit* buffer = new Bit[len];

        for (size_t i = 0; i < len; i++) {
            buffer[len - i - 1] = prefix_state % 2;
            prefix_state >>= 1;
        }

        obs.puts(buffer, len);

        if (!feof(ifp)) {
            obs.putByte(buf);
        }

#ifdef LZ78_BYTE_ENCODER_DEBUG_WRITE

        for (size_t i = 0; i < len; i++) {
            printf("%d", buffer[i]);
        }

        if (!feof(ifp)) {
            printf(" %2x", buf);
        }

        cout << endl;
#endif // LZ78_BYTE_ENCODER_DEBUG_WRITE

        delete[] buffer;
        buffer = NULL;
    }

    obs.dumpLength();
}

SSIZE LZ78ByteDecoder::partition_cnt()
{
    return lb(ibs.extractLength());
}

void LZ78ByteDecoder::decode()
{
    std::vector<Code*> table;
    table.reserve(size_t(partition_cnt() + 1));
    table.push_back(nullptr);

    Bit* prefixbuf = nullptr;
    Byte tail;

    size_t len = 1, mcnt = 2;
    size_t prefix_state;

    while (ibs.gets(prefixbuf, len)) {
        // Extract the prefix state code8
        prefix_state = prefixbuf[0];
        for (size_t i = 1; i < len; i++) {
            prefix_state = (prefix_state << 1) + prefixbuf[i];
        }

        ibs.getByte(tail);

#ifdef LZ78_BYTE_DECODER_DEBUG_WRITE
        DEBUG_HEADER;
#endif // LZ78_BYTE_DECODER_DEBUG_WRITE

        if (!ibs.b_eof()) {
            Code* ptr = new Code();
            table.push_back(ptr);

            if (table[prefix_state] != nullptr) {
                // if prefix exists
                Code* prefix = table[prefix_state];

                ptr->pattern_len = prefix->pattern_len + 1;
                ptr->pattern = new Byte[ptr->pattern_len];
                memcpy_s(ptr->pattern, ptr->pattern_len, prefix->pattern, prefix->pattern_len);
                ptr->pattern[prefix->pattern_len] = tail;
            } else {
                // if prefix does not exist
                ptr->pattern_len = 1;
                ptr->pattern = new Byte[1];
                ptr->pattern[0] = tail;
            }

            fwrite(ptr->pattern, sizeof(Byte), ptr->pattern_len, ofp);

#ifdef LZ78_BYTE_DECODER_DEBUG_WRITE
            for (size_t i = 0; i < ptr->pattern_len; i++) {
                printf("%2x ", ptr->pattern[i]);
            }
#endif // LZ78_BYTE_DECODER_DEBUG_WRITE

        } else if (table[prefix_state] != nullptr) {
            Code* prefix = table[prefix_state];

            fwrite(prefix->pattern, sizeof(Byte), prefix->pattern_len, ofp);

#ifdef LZ78_BYTE_DECODER_DEBUG_WRITE
            for (size_t i = 0; i < prefix->pattern_len; i++) {
                printf("%2x ", prefix->pattern[i]);
            }
#endif // LZ78_BYTE_DECODER_DEBUG_WRITE

            break;
        }

#ifdef LZ78_BYTE_DECODER_DEBUG_WRITE
        cout << endl;
#endif // LZ78_BYTE_DECODER_DEBUG_WRITE

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
