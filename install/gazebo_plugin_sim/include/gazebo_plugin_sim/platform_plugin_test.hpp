#ifndef GAZEBO_PLUGINS__PLATFORM_PLUGIN_TEST_HPP_
#define GAZEBO_PLUGINS__PLATFORM_PLUGIN_TEST_HPP_

#include <gazebo/common/Plugin.hh>

#include <memory>

namespace gazebo_plugin_sim
{
class PlatformPluginTestPrivate;

class PlatformPluginTest : public gazebo::ModelPlugin
{
public:
  /// Constructor
  PlatformPluginTest();

  /// Destructor
  ~PlatformPluginTest();

protected:
  // Documentation inherited
  void Load(gazebo::physics::ModelPtr _model, sdf::ElementPtr _sdf) override;

  // Documentation inherited
  void Reset() override;

private:
  /// Private data pointer
  std::unique_ptr<PlatformPluginTestPrivate> impl_;
};
}  // namespace gazebo_plugin_sim

#endif  // GAZEBO_PLUGINS__PLATFORM_PLUGIN_TEST_HPP_