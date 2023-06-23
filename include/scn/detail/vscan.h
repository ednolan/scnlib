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

#pragma once

#include <scn/detail/context.h>
#include <scn/detail/istream_range.h>
#include <scn/detail/result.h>

namespace scn {
    SCN_BEGIN_NAMESPACE

    template <typename Range, typename CharT>
    using scan_arg_for = basic_scan_arg<
        basic_scan_context<detail::decayed_mapped_source_range<Range>, CharT>>;
    template <typename Range, typename CharT>
    using scan_args_for = basic_scan_args<
        basic_scan_context<detail::decayed_mapped_source_range<Range>, CharT>>;

    namespace detail {
        scan_result<std::string_view> vscan_impl(
            std::string_view source,
            std::string_view format,
            scan_args_for<std::string_view, char> args);
        scan_result<erased_subrange> vscan_impl(
            erased_subrange source,
            std::string_view format,
            scan_args_for<erased_subrange, char> args);
#if SCN_USE_IOSTREAMS
        scan_result<istreambuf_subrange> vscan_impl(
            istreambuf_subrange source,
            std::string_view format,
            scan_args_for<istreambuf_subrange, char> args);
#endif

        template <typename Locale>
        scan_result<std::string_view> vscan_localized_impl(
            const Locale& loc,
            std::string_view source,
            std::string_view format,
            scan_args_for<std::string_view, char> args);
        template <typename Locale,
                  typename = std::void_t<decltype(Locale::classic())>>
        scan_result<erased_subrange> vscan_localized_impl(
            Locale& loc,
            erased_subrange source,
            std::string_view format,
            scan_args_for<erased_subrange, char> args);
#if SCN_USE_IOSTREAMS
        template <typename Locale,
                  typename = std::void_t<decltype(Locale::classic())>>
        scan_result<istreambuf_subrange> vscan_localized_impl(
            Locale& loc,
            istreambuf_subrange source,
            std::string_view format,
            scan_args_for<istreambuf_subrange, char> args);
#endif

        scan_result<std::string_view> vscan_value_impl(
            std::string_view source,
            scan_arg_for<std::string_view, char> arg);
        scan_result<erased_subrange> vscan_value_impl(
            erased_subrange source,
            scan_arg_for<erased_subrange, char> arg);
#if SCN_USE_IOSTREAMS
        scan_result<istreambuf_subrange> vscan_value_impl(
            istreambuf_subrange source,
            scan_arg_for<istreambuf_subrange, char> arg);
#endif

#if SCN_USE_IOSTREAMS
        scan_result<istreambuf_subrange> vscan_and_sync_impl(
            istreambuf_subrange source,
            std::string_view format,
            scan_args_for<istreambuf_subrange, char> args);
#endif
    }  // namespace detail

    template <typename Range>
    auto vscan(Range&& range,
               std::string_view format,
               scan_args_for<Range, char> args)
    {
        auto mapped_range = detail::scan_map_input_range(range);
        auto result = detail::vscan_impl(mapped_range, format, args);
        auto result_range =
            detail::map_scan_result_range(SCN_FWD(range), result.range());
        return scan_result{SCN_MOVE(result_range), result.error()};
    }

    template <typename Range,
              typename Locale,
              typename = std::void_t<decltype(Locale::classic())>>
    auto vscan(const Locale& loc,
               Range&& range,
               std::string_view format,
               scan_args_for<Range, char> args)
    {
        auto mapped_range = detail::scan_map_input_range(range);
        auto result =
            detail::vscan_localized_impl(loc, mapped_range, format, args);
        auto result_range =
            detail::map_scan_result_range(SCN_FWD(range), result.range());
        return scan_result{SCN_MOVE(result_range), result.error()};
    }

    template <typename Range>
    auto vscan_value(Range&& range, scan_arg_for<Range, char> arg)
    {
        auto mapped_range = detail::scan_map_input_range(range);
        auto result = detail::vscan_value_impl(mapped_range, arg);
        auto result_range =
            detail::map_scan_result_range(SCN_FWD(range), result.range());
        return scan_result{SCN_MOVE(result_range), result.error()};
    }

    template <typename Range>
    auto vscan_and_sync(Range&& range,
                        std::string_view format,
                        scan_args_for<Range, char> args)
    {
        auto mapped_range = detail::scan_map_input_range(range);
        auto result = detail::vscan_and_sync_impl(mapped_range, format, args);
        auto result_range =
            detail::map_scan_result_range(SCN_FWD(range), result.range());
        return scan_result{SCN_MOVE(result_range), result.error()};
    }

    SCN_END_NAMESPACE
}  // namespace scn
