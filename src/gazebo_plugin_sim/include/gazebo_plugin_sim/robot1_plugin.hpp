#ifndef GAZEBO_PLUGINS__ROBOT1_PLUGIN_HPP_
#define GAZEBO_PLUGINS__ROBOT1_PLUGIN_HPP_

#include <gazebo/common/Plugin.hh>

#include <memory>

namespace gazebo_plugin_sim
{
class Robot1PluginPrivate;

class Robot1Plugin : public gazebo::ModelPlugin
{
public:
  /// Constructor
  Robot1Plugin();

  /// Destructor
  ~Robot1Plugin();

protected:
  // Documentation inherited
  void Load(gazebo::physics::ModelPtr _model, sdf::ElementPtr _sdf) override;

  // Documentation inherited
  void Reset() override;

private:
  /// Private data pointer
  std::unique_ptr<Robot1PluginPrivate> impl_;
};
}  // namespace gazebo_plugin_sim

#endif  // GAZEBO_PLUGINS__ROBOT1_PLUGIN_HPP_