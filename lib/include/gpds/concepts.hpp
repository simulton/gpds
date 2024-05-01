#include <concepts>
#include <string_view>

namespace gpds
{

    class container;

    template<typename T>
    concept Serializable = requires(T t, gpds::container c)
    {
        { T::gpds_name } -> std::convertible_to<std::string_view>;

        { t.to_container() } -> std::same_as<gpds::container>;
        { t.from_container(c) } -> std::same_as<void>;
    };

}
