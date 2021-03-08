#include <iostream>
#include <vector>

#include <sdf/sdf.hh>

int main(int argc, const char* argv[])
{
  // check arguments
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0]
              << " <sdf-path>" << std::endl;
    return -1;
  }

  // store sdf filename from command line arguement
  // into sdfPath
  const std::string sdfPath(argv[1]);

  // load and check sdf file
  // parse the file into sdfElement
  sdf::SDFPtr sdfElement(new sdf::SDF());
  sdf::init(sdfElement);
  if (!sdf::readFile(sdfPath, sdfElement))
  {
    std::cerr << sdfPath << " is not a valid SDF file!" << std::endl;
    return -2;
  }

  // start parsing model
  const sdf::ElementPtr rootElement = sdfElement->Root();
  if (!rootElement->HasElement("model"))
  {
    std::cerr << sdfPath << " is not a model SDF file!" << std::endl;
    return -3;
  }
  const sdf::ElementPtr modelElement = rootElement->GetElement("model");
  const std::string modelName = modelElement->Get<std::string>("name");
  std::cout << "Found " << modelName << " model!" << std::endl;

  // parse model links
  sdf::ElementPtr linkElement = modelElement->GetElement("link");
  while (linkElement)
  {
    // find pose of the link
    const std::string linkName = linkElement->Get<std::string>("name");
    const sdf::ElementPtr linkPose = linkElement -> GetElement("pose");
    const sdf::ParamPtr linkPoseFrame = linkPose -> GetAttribute("frame");
    const sdf::ParamPtr linkPoseValue = linkElement -> GetValue(); // NULL
    std::string vp = "1 2 3 4 5 56";
    if (linkPose -> Set(vp)) {std::cout << "set!";}
    else {std::cout << "aba not set";}
    if (!linkPoseValue) std::cout << "link pose's value is NULL";
    std::cout << "requirement" << linkPose -> GetRequired();
    std::cout << "print link pose values:" << std::endl;
    linkPose -> PrintValues("link pose value:");
    std::cout << "and its description" << linkPose -> GetDescription()
	      << std::endl;
    std::cout << "Found " << linkName << "link in "
	      << modelName << " has pose" << linkPose << std::endl;
    std::cout << "Found " << linkName << " link in "
              << modelName << " model!" << std::endl;
    if (linkPoseFrame != NULL){
      std::string desc = linkPoseFrame -> GetDescription();
      const std::string linkPoseFrameStr = linkPoseFrame -> GetAsString();
      int len = linkPoseFrameStr.length();
      const std::string linkPoseFrameType = linkPoseFrame -> GetTypeName();
      
      std::cout << "got string of the attribute" << linkPoseFrameStr
		<< "with lenght" << len
	        << std::endl
		<< "and type" << linkPoseFrameType
		<< std::endl;
    }else
      {
	std::cout << "Failed to get attribute frame";
	exit(-1);
      }
    std::cout << "Found parameter of attribute pose, description:"
	      <<linkPoseFrame -> GetDescription()
	      << std::endl;
    // printValues
    linkElement -> PrintValues(linkName);
    linkElement = linkElement->GetNextElement("link");
  }

  // parse model joints
  sdf::ElementPtr jointElement = modelElement->GetElement("joint");
  while (jointElement)
  {
    const std::string jointName = jointElement->Get<std::string>("name");
    std::cout << "Found " << jointName << " joint in "
              << modelName << " model!" << std::endl;

    const sdf::ElementPtr parentElement = jointElement->GetElement("parent");
    const std::string parentLinkName = parentElement->Get<std::string>();

    const sdf::ElementPtr childElement = jointElement->GetElement("child");
    const std::string childLinkName = childElement->Get<std::string>();

    std::cout << "Joint " << jointName << " connects " << parentLinkName
              << " link to " << childLinkName << " link" << std::endl;

    jointElement = jointElement->GetNextElement("joint");
  }

  std::cout << "the root:element" << std::endl;
  rootElement -> PrintValues("root");
  return 0;
}
