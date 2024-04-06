module;

#include <algorithm>
#include <format>
#include <functional>
#include <initializer_list>
#include <ranges>
#include <sstream>
#include <stacktrace>
#include <string_view>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>

export module stl;

export namespace std {

namespace views = ranges::views;

    namespace ranges::views {

    using std::views::enumerate;
    using std::views::drop;

    using std::ranges::_Pipe::operator|;

    }

using std::reference_wrapper;
using std::initializer_list;
using std::stacktrace;
using std::tuple;
using std::variant;
using std::string_view;
using std::string;
using std::conditional_t;
using std::remove_reference_t;
using std::stringstream;

using std::is_same_v;
using std::in_place_type;
using std::is_void_v;
using std::is_reference_v;
using std::is_constructible_v;
using std::is_move_constructible_v;
using std::is_move_assignable_v;
using std::is_copy_constructible_v;
using std::is_copy_assignable_v;
using std::is_nothrow_assignable_v;
using std::is_nothrow_constructible_v;
using std::is_nothrow_move_constructible_v;
using std::is_nothrow_move_assignable_v;

using std::operator<<;

using std::forward;
using std::vformat;
using std::make_format_args;
using std::move;
using std::holds_alternative;
using std::get;

}
