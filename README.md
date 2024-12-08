# SecuritySystem Project

This repository contains the source code for the SecuritySystem project, developed for the Embedded Software for the Internet of Things course at the University of Trento. The project is designed using Code Composer Studio (Version 12.7.1.00001) and includes all necessary dependencies, which are already present in the repository.  

Follow the instructions below to set up the CCS project.

---

## Setting Up the CCS Project

1. **Clone the Repository**

2. **Set the Workspace in Code Composer Studio (CCS)**
  - Launch Code Composer Studio.
  - When prompted, set the workspace directory to:
    ```
      <project_root>/central/workspace
    ```
    Replace <project_root> with the path to the root folder of the cloned repository.

3. **Configure the Path Variable for Libraries**
  To set up the library path variable in CCS:
    - Navigate to Window → Preferences.
    - In the preferences window:
      - Go to Code Composer Studio → Build → Variables.
      - Click on "New" to create a new variable.
    - Configure the variable as follows:
      - Name: IOT_PROJECT_ROOT
      - Type: Directory
      - Value: Set this to the root folder of the project. By default, this is:
        ```
            SecuritySystem
        ```

## Notes

- On Windows, errors might occur due to excessively long pathnames. To resolve this issue, you can follow the solution outlined in this [Stack Overflow thread](https://stackoverflow.com/questions/22575662/filename-too-long-in-git-for-windows).  
  Specifically, refer to the "Appendix - Doing it all from PowerShell - the copy-paste edition" section.
