#include "fragment.hpp"
#include <memory>
#include <algorithm>
#include <3rdparty/tinyxml2-ex/tixml2ex.h>
#include "value.hpp"

namespace tinyxml2
{
    class XMLElement;
    class XMLDocument;
}

namespace gpds
{
    class document_xml;

    class fragment_xml : public fragment
    {
        friend document_xml;
    public:
        explicit fragment_xml(tinyxml2::XMLElement* element);
        explicit fragment_xml(tinyxml2::XMLDocument* document);
        [[nodiscard]] virtual std::vector<std::string> query_list(const std::string& qry) const override;
        [[nodiscard]] virtual std::string query(const std::string& qry) const override;
        [[nodiscard]] virtual std::vector<value> query_values(const std::string& qry) const override;
        [[nodiscard]] virtual value query_value(const std::string& qry) const override;
        [[nodiscard]] virtual std::vector<std::unique_ptr<fragment>> query_fragments(const std::string& qry) const override;
        [[nodiscard]] virtual std::unique_ptr<fragment> query_fragment(const std::string& qry) const override;
        [[nodiscard]] virtual container to_container() const override;

    private:
        [[nodiscard]] tinyxml2::Selector<tinyxml2::XMLElement> query_elements(const std::string& qry) const;

        // The fragment can either represent an element or a whole document.
        std::variant<tinyxml2::XMLElement*, tinyxml2::XMLDocument*> m_element;
    };
}
