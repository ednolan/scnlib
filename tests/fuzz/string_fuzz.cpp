// Copyright 2017 Elias Kosunen
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file is a part of scnlib:
//     https://github.com/eliaskosunen/scnlib

#include "fuzz.h"

namespace scn::fuzz {
template <typename CharT, typename Source>
void do_basic_run_for_source(Source& source,
                             format_strings_view<CharT> format_strings)
{
    do_basic_run_for_type<CharT, std::basic_string<CharT>>(source,
                                                           format_strings);
    if constexpr (scn::ranges::contiguous_range<Source>) {
        do_basic_run_for_type<CharT, std::basic_string_view<CharT>>(
            source, format_strings);
    }
}

namespace {
void run(span<const uint8_t> data)
{
    if (data.size() > max_input_bytes || data.size() == 0) {
        return;
    }

    auto [sv, wsv_direct, wsv_reinterpret, wsv_transcode] =
        make_input_views(data);

    auto f = get_format_strings<char>("{}", "{:L}", "{:s}", "{:64c}", "{:64U}",
                                      "{:[A-Za-z]}");
    do_basic_run(sv, f);

    auto wf = get_format_strings<wchar_t>(L"{}", L"{:L}", L"{:s}", L"{:64c}",
                                          L"{:64U}", L"{:[A-Za-z]}");
    do_basic_run(wsv_direct, wf);
    do_basic_run(wsv_reinterpret, wf);
    do_basic_run(wsv_transcode, wf);
}
}  // namespace
}  // namespace scn::fuzz

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    scn::fuzz::run({data, size});
    return 0;
}
