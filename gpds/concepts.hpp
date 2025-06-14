#include <concepts>
#include <string_view>

namespace gpds
{

    class container;

    template<typename T>
    concept Serializable = requires(T t)
    {
        { T::gpds_name } -> std::convertible_to<std::string_view>;

        { t.to_container() } -> std::same_as<gpds::container>;
    };

    template<typename T>
    concept Deserializable = requires(T t, gpds::container c)
    {
        { T::gpds_name } -> std::convertible_to<std::string_view>;

        { t.from_container(c) } -> std::same_as<void>;
    };

    template<typename T>
    concept SerializableRange = std::ranges::forward_range<T> && Serializable<std::ranges::range_value_t<T>>;

}
