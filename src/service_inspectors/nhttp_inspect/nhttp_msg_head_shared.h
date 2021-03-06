/*
** Copyright (C) 2014 Cisco and/or its affiliates. All rights reserved.
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License Version 2 as
** published by the Free Software Foundation.  You may not use, modify or
** distribute this program under any other version of the GNU General
** Public License.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
// nhttp_msg_head_shared.h author Tom Peters <thopeter@cisco.com>

#ifndef NHTTP_MSG_HEAD_SHARED_H
#define NHTTP_MSG_HEAD_SHARED_H

#include "nhttp_str_to_code.h"
#include "nhttp_head_norm.h"
#include "nhttp_msg_section.h"
#include "nhttp_field.h"

//-------------------------------------------------------------------------
// NHttpMsgHeadShared class
//-------------------------------------------------------------------------

class NHttpMsgHeadShared: public NHttpMsgSection {
public:
    void analyze() override;
    void gen_events() override;

    int32_t get_num_headers() const { return num_headers; };
    const Field& get_headers() const { return msg_text; };
    const Field& get_header_line(int k) const { return header_line[k]; };
    const Field& get_header_name(int k) const { return header_name[k]; };
    const Field& get_header_value(int k) const { return header_value[k]; };
    NHttpEnums::HeaderId get_header_name_id(int k)  const { return header_name_id[k]; };
    const Field& get_header_value_norm(NHttpEnums::HeaderId header_id);

protected:
    NHttpMsgHeadShared(const uint8_t *buffer, const uint16_t buf_size, NHttpFlowData *session_data_,
       NHttpEnums::SourceId source_id_, bool buf_owner) :
       NHttpMsgSection(buffer, buf_size, session_data_, source_id_, buf_owner) {};

    // Header normalization strategies. There should be one defined for every different way we can process
    // a header field value.
    static const HeaderNormalizer NORMALIZER_NIL;
    static const HeaderNormalizer NORMALIZER_BASIC;
    static const HeaderNormalizer NORMALIZER_CAT;
    static const HeaderNormalizer NORMALIZER_NOREPEAT;
    static const HeaderNormalizer NORMALIZER_DECIMAL;
    static const HeaderNormalizer NORMALIZER_TRANSCODE;

    // Master table of known header fields and their normalization strategies.
    static const HeaderNormalizer* const header_norms[];

    // Tables of header field names and header value names
    static const StrCode header_list[];
    static const StrCode trans_code_list[];

    void parse_header_block();
    static uint32_t find_header_end(const uint8_t* buffer, int32_t length, int* const num_seps);
    void parse_header_lines();
    void derive_header_name_id(int index);

    void print_headers(FILE *output);

    // All of these are indexed by the relative position of the header field in the message
    static const int MAXHEADERS = 200;  // I'm an arbitrary number. Need to revisit.
    int32_t num_headers = NHttpEnums::STAT_NOTCOMPUTE;
    Field header_line[MAXHEADERS];
    Field header_name[MAXHEADERS];
    NHttpEnums::HeaderId header_name_id[MAXHEADERS];
    Field header_value[MAXHEADERS];

    // Normalized values are indexed by HeaderId
    int header_count[NHttpEnums::HEAD__MAXVALUE] = { };
    Field header_value_norm[NHttpEnums::HEAD__MAXVALUE];
};

#endif



















