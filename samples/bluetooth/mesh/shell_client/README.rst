.. _bluetooth_mesh_shell_client:

Bluetooth: Mesh shell client
#############################

.. contents::
   :local:
   :depth: 2

The Bluetooth® shell client sample demonstrates how to set up various client models and how to use shell commands to send messages to their respective servers.

Use the shell client sample with the :ref:`bluetooth_mesh_light_lc` sample to demonstrate its function in a Bluetooth mesh network.

Requirements
************
The sample supports the following development kits:

.. table-from-sample-yaml::

You need at least two development kits:

* One development kit where you program this sample application (the clients).
* One (or more) development kit(s) where you program the :ref:`bluetooth_mesh_light_lc` sample application (the servers), and configure according to the mesh light fixture sample's :ref:`testing guide <bluetooth_mesh_light_ctrl_testing>`.

The sample also requires a smartphone with Nordic Semiconductor's nRF Mesh mobile app installed in one of the following versions:

  * `nRF Mesh mobile app for Android`_
  * `nRF Mesh mobile app for iOS`_

.. include:: /includes/tfm.txt

Overview
********
This sample instantiates the :ref:`bt_mesh_scheduler_cli_readme`, :ref:`bt_mesh_scene_cli_readme`, :ref:`bt_mesh_time_cli_readme`, and the :ref:`bt_mesh_light_ctrl_cli_readme` (Light LC Client).
It demonstrates how to control the states of a :ref:`bt_mesh_time_srv_readme`, a :ref:`bt_mesh_scene_srv_readme`, a :ref:`bt_mesh_scheduler_srv_readme` and a :ref:`bt_mesh_light_ctrl_srv_readme` (Light LC Server) through the use of Client messages sent through shell.
The sample shows a method for setting the time state of the Time Server, and how to use the Scene Server to store the state of a node.
Additionally, the sample shows how to use the Scheduler Server to schedule periodic on/off tasks as well as how to periodically recall scenes.
The Light LC Client is used to set up a scene to be stored.

Devices are nodes with a provisionee role in a mesh network.
Provisioning is performed using the `nRF Mesh mobile app`_.
This mobile application is also used to configure key bindings, and publication and subscription settings of the Bluetooth mesh model instances in the sample.
After provisioning and configuring the mesh models supported by the sample in the `nRF Mesh mobile app`_, you can control the dimmable LED on the development kit from the app.

Provisioning
============

The provisioning is handled by the :ref:`bt_mesh_dk_prov`.
It supports four types of out-of-band (OOB) authentication methods, and uses the Hardware Information driver to generate a deterministic UUID to uniquely represent the device.

Models
======

The following table shows the mesh shell client composition data for this sample:

.. table::
   :align: center

   +-------------------------------+
   | Element 1                     |
   +===============================+
   | Config Server                 |
   +-------------------------------+
   | Health Server                 |
   +-------------------------------+
   | Scheduler Client              |
   +-------------------------------+
   | Time Client                   |
   +-------------------------------+
   | Scene Client                  |
   +-------------------------------+
   | Light LC Client               |
   +-------------------------------+

The models are used for the following purposes:

* The element contains a Config Server and a Health Server.
  The Config Server allows configurator devices to configure the node remotely.
  The Health Server provides ``attention`` callbacks that are used during provisioning to call your attention to the device.
  These callbacks trigger blinking of the LEDs.
* The remaining models are instances of the various clients described in the overview.
  Lists of the available shell commands for the client models can be found on their :ref:`model pages <bt_mesh_models>`.

The model handling is implemented in :file:`src/model_handler.c`, which uses the :ref:`dk_buttons_and_leds_readme` library to control the LEDs on the development kit.

User interface
**************

Buttons:
   Can be used to input the out-of-band (OOB) authentication value during provisioning.
   All buttons have the same functionality during this procedure.

LEDs:
   Show the OOB authentication value during provisioning if the "Push button" OOB method is used.
   First LED outputs the current light level of the Light Lightness Server in the first element.


Configuration
*************

|config|

Source file setup
=================

This sample is split into the following source files:

* A :file:`main.c` file to handle initialization.
* A file for handling mesh models, :file:`model_handler.c`.
* A python script :file:`time_set.py` that contains a function that can be used to set the time status of a Time Server.
* A python script :file:`toggle_onoff.py` that can be used to schedule periodic tasks to toggle a LED every 15s.
* A python script :file:`toggle_scene.py` that can be used to schedule periodic tasks to recall a scene every 30s.
* A python script :file:`store_scenes.py` that can be used to generate scenes to be used in the point above.


FEM support
===========

.. include:: /includes/sample_fem_support.txt

Building and running
********************

.. |sample path| replace:: :file:`samples/bluetooth/mesh/shell_client`

.. include:: /includes/build_and_run_ns.txt

.. _bluetooth_mesh_shell_client_testing:

Testing
=======

After programming the sample to your development kit, you can test it by using a smartphone with `nRF Mesh mobile app`_ installed.
Testing consists of provisioning the device and configuring it for communication with the mesh models.

Provisioning the device
-----------------------

.. |device name| replace:: :guilabel:`Mesh Shell Client`

.. include:: /includes/mesh_device_provisioning.txt

Configuring models
------------------

See :ref:`ug_bt_mesh_model_config_app` for details on how to configure the mesh models with the nRF Mesh mobile app.

Configure each of the Client models on the :guilabel:`Mesh Shell Client` node:

* Bind the model to :guilabel:`Application Key 1`.
* Set the publication parameters:

  * Destination/publish address: Set the :guilabel:`Publish Address` of the Client model to the corresponding Server unicast address on the :guilabel:`Mesh Light Fixture` node.
  * Retransmit count: Set the count to zero (:guilabel:`Disabled`), to prevent the model from sending each message multiple times.

Running python scripts
----------------------
.. note::
  The python scripts uses serial communication, and needs to be configured with the COM port that the :guilabel:`Mesh Shell Client` is connected to.

The scripts can be run from an independent terminal.


.. note::
   The state machine of a Light LC server will get disabled when something other than a itself is used to control a Light Lightness Server, such as when scheduling periodic on/off tasks.
   The state machine can be re-enabled by sending a ``mode-set`` message from the Light LC Client.

Dependencies
************

This sample uses the following |NCS| libraries:

* :ref:`bt_mesh_scheduler_cli_readme`
* :ref:`bt_mesh_time_cli_readme`
* :ref:`bt_mesh_scene_cli_readme`
* :ref:`bt_mesh_light_ctrl_cli_readme`
* :ref:`bt_mesh_dk_prov`
* :ref:`dk_buttons_and_leds_readme`

In addition, it uses the following Zephyr libraries:

* ``include/drivers/hwinfo.h``
* :ref:`zephyr:kernel_api`:

  * ``include/kernel.h``

* :ref:`zephyr:bluetooth_api`:

  * ``include/bluetooth/bluetooth.h``

* :ref:`zephyr:bluetooth_mesh`:

  * ``include/bluetooth/mesh.h``
