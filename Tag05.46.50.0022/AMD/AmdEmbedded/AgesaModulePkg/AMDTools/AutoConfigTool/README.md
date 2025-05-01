# Auto Config Tools

## Background

In the current AGESA implementation, there are many mechanisms for configuring AGESA settings, including CBS options, APCB tokens, and PCDs.  In addition to those configuration mechanisms, the settings themselves may be updated during build or during boot to reflect the actual value in use.  At the moment, there is no easy way for the developers to determine the actual settings in use during boot due to how many factors there are in determining the effective value.  In addition to these challenges, many settings are set by default to an "Auto" value.  The "Auto" value (typically 0xFF) is then translated into a known good value during boot.  However, without looking at the code, it is difficult, if not impossible, to determine what the actual value in use is.

This Auto Config Tool (ACT) addresses the issue of being able to read the current, effective settings on a booted target.  There are a few basic components of the design of the ACT toolset:
1. ACT Exporter
   The ACT Exporter uses a BIOS Setup XML to create a header file to be used during BIOS boot as well as a python dictionary file that will be used with the ACT Shell Tool to display and report on actual boot values.

2. BOOT Component
   During boot, BIOS uses a known set of UID-to-Setting pairs to save the actual data values during boot.

3. ACT Shell Reporter
   TODO
   The ACT Shell Reporter shall use the python dictionary file created by the ACT Exporter to decode the UID setting pairs saved during boot.


## Required Tools

* [Git](https://git-scm.com/)
  Make sure any proxy requirements are set in the git config settings.

* [Python3](https://www.python.org)
  Environment Variable: PYTHON_HOME
  Version 3.8+
  ```
  pip install mmh3
  pip install pyyaml
  ```


## ACT Exporter

The ACT exporter parses an XML file for settings information used to generate a header file for use in bios and a Python dictionary for use by a Shell application.

### Usage

This script assumes correct paths are used in YAML file.
This script must be called from non-git folder.

```
python ActOptionExporter.py --cfg <yaml_config_file>
```

