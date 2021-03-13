// read from text file cone_pose containing 2 floats on each line:
// x y 
// read SDF model from model.sdf and modify the values of the elements
// so that the poses of the cones match those specified in cone_pose
#include "sdf/Element.hh"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <sdf/sdf.hh>
#include <stdio.h>
#include <vector>

// #define DEBUGG

using namespace std;
void           mod_cone_pose(sdf::ElementPtr root, vector<string> poses);
vector<string> read_lines(const string &);
vector<string> split_by_space(const string &);
bool           islegal_line(const string &);
bool           isfloat_string(const std::string &);
void           test_islegal_line();
void           alter_pose(const sdf::ElementPtr &, const vector<string> &);
const sdf::ElementPtr get_sdf_root(const string &);

int main(int argc, char **argv)
{
    if (argc != 3)
        {
            cout << "usage: " << argv[0] << " input_poses"
                 << " input_sdf";
            exit(-1);
        }

    vector<string> lines = read_lines(argv[1]);
#ifdef DEBUGG
    cout << "read lines:\n";
#endif
    for (auto it : lines)
        {
            if (islegal_line(it))
                {
#ifdef DEBUGG
                    cout << it << endl;
#endif
                }
            else
                {
                    cerr << "Illegal input pose format:\n" << it;
                }
        }

    string          input_sdf_filename = argv[2];
    sdf::ElementPtr root               = get_sdf_root(input_sdf_filename);
    alter_pose(root, lines);
    root->PrintValues("");
    return 0;
}

vector<string> read_lines(const string &filename)
{
    ifstream       ifs(filename);
    vector<string> lines;
    if (!ifs)
        {
            cerr << "unable to open file" << filename << endl;
        }
    else
        {
            for (string line; getline(ifs, line); /**/)
                {
                    lines.push_back(line);
                }
        }
    return lines;
}

vector<string> split_by_space(const string &str)
{
    vector<string> maybe_numbers;
    boost::split(maybe_numbers, str, boost::is_any_of(" "));
    return maybe_numbers;
}

void test_islegal_line()
{
    string         l1            = "1.0 3.0 5.0 4.8 1.3";
    string         l2            = "aba ababa 1.0";
    string         l3            = "3.0 3.3 e";
    string         l4            = "a sdf sdf sdf sdfj";
    string         l5            = "1.0 3.0 5.0 .22 4.8 1.3";
    vector<string> lines_to_test = {l1, l2, l3, l4, l5};
    for (auto l : lines_to_test)
        {
            cout << "testing with line" << l << "...\n";
            bool b = islegal_line(l);
            cout << "-----> " << b << endl;
        }
}

bool islegal_line(const string &line)
{
    vector<string> maybe_numbers = split_by_space(line);
    if (maybe_numbers.size() != 2)
        {
            return false;
        }
    bool ret = true;
    for (auto maybe_number : maybe_numbers)
        {
            if (!isfloat_string(maybe_number))
                {
                    ret = false;
                    break;
                }
        }
    return ret;
}

bool isfloat_string(const std::string &someString)
{
    using boost::bad_lexical_cast;
    using boost::lexical_cast;

    try
        {
            boost::lexical_cast<float>(someString);
        }
    catch (bad_lexical_cast &)
        {
            return false;
        }

    return true;
}

const sdf::ElementPtr get_sdf_root(const string &sdf_filename)
{
    sdf::SDFPtr sdfElement(new sdf::SDF());
    sdf::init(sdfElement);
    if (!sdf::readFile(sdf_filename, sdfElement))
        {
            cerr << sdf_filename << " is not a valid SDF file!" << std::endl;
            return NULL;
        }

    // start parsing model
    const sdf::ElementPtr rootElement = sdfElement->Root();
    if (!rootElement->HasElement("model"))
        {
            cerr << sdf_filename << " is not a model SDF file!" << std::endl;
            return NULL;
        }
    return rootElement;
}

void alter_pose(const sdf::ElementPtr &root, const vector<string> &lines)
{
    // obtain the first link Element
    const sdf::ElementPtr model     = root->GetElement("model");
    const std::string     modelName = model->Get<std::string>("name");
#ifdef DEBUGG
    std::cout << "Found " << modelName << " model!" << std::endl;
#endif
    sdf::ElementPtr link = model->GetElement("link");

    // set name of the link
    int  i                = 0;
    int  lines_num        = lines.size();
    bool print_link_value = true;

    for (i = 0; i < lines_num - 1; i++)
        {
            const sdf::ElementPtr linkPose = link->GetElement("pose");
            string        name = string("cone_") + std::to_string(i + 1);
            sdf::ParamPtr link_name_param = link->GetAttribute("name");
            link_name_param->SetFromString(name);
	    string link_pose_str = lines[i] + " 0 0 0 0";

            if (linkPose->Set(link_pose_str))
                {
#ifdef DEBUGG
                    cout << "name and pose set for " << i + 1 << "th cone\n";
#endif
                }
            else
                {
                    cerr << "cannot set pose, abaaba....";
                }
            if (i != lines_num - 2)
                {
                    // string name_next_cone =
                    // string("cone_") + std::to_string(i +
                    // 2); link->AddElement(name_next_cone);
                    sdf::ElementPtr next_link = link->Clone();
                    root->InsertElement(next_link);
                    link = next_link;
                }
        }
}
