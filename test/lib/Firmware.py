import os
import subprocess
import filecmp

class Firmware(object):
    DEFAULT_REV = "8a00"
    def __init__(self):
        self.main_path = '\\'.join(os.path.realpath(__file__).split('\\')[:-3])  # Main path of the whole project including firmware, test, etc.
        self.build_path = self.main_path + "\\build"                # Path of the test folder
        print(f"Main path: {self.main_path}")
        print(f"Build path: {self.build_path}")

    def readsym(self, sym_file=None):
        """
        Read the symbol file and return the dictionary of the symbols
        :param sym_file: Symbol file path
        :return: Dictionary of the symbols
        """
        symbols = {}
        # If the symbol file is not provided, use the default one
        if not sym_file:
            sym_file = self.build_path + "\\main.sym"

        # Read the symbol file
        with open(sym_file, 'r') as file:
            for line in file:
                line = line.strip()
                if line:
                    symbol, value = line.split(' ')
                    symbols[symbol] = value

        # Return the symbols dictionary
        return symbols

    def readmacro(self, macro_file=None):
        """
        Read the macro file and return the dictionary of the macros
        :param macro_file: Macro file path
        :return: Dictionary of the macros
        """
        macros = {}
        # If the macro file is not provided, use the default one
        if not macro_file:
            macro_file = self.build_path + "\\main.macro"

        # Read the macro file
        with open(macro_file, 'r') as file:
            for line in file:
                line = line.strip()
                if line:
                    if line.startswith("#define"):
                        # Parse the line and extract the macro and value
                        if len(line.split(' ')) == 3:
                            macro, value = line.split(' ')[1:]

                            if self.isint(value):
                                macros[macro] = int(value)
                            elif self.ishex(value):
                                macros[macro] = int(value, 16)
                            else:
                                macros[macro] = value
        # Return the macros dictionary
        return macros

    def build(self, build_option="blink_a"):
        subprocess.run("make " + build_option, cwd=self.main_path)

    def release(self, build_option="blink_a", commit_id=None):
        # Check out to a specific commit
        if commit_id is not None:
            self.git_checkout(commit_id)
        # Release the firmware with zip file
        subprocess.run("make " + build_option + f" rel=3 rev={self.DEFAULT_REV}", cwd=self.main_path)
        # Get commit ID from GIT
        commit_id = self.git_info()['id']
        # Rename the zip file by build option and commit ID
        zip_file = self.main_path + f"\\release\\{build_option}\\{build_option}_{commit_id}.zip"
        if os.path.exists(zip_file):
            os.remove(zip_file)
        os.rename(self.main_path + f"\\release\\{build_option}_{self.DEFAULT_REV}.zip", zip_file)
        # Remove the old build folder specied by {self.DEFAULT_REV}
        subprocess.run(f"rmdir /s /q {self.main_path}\\release\\{build_option}\\{self.DEFAULT_REV}", shell=True)

    def compare(self, build_option="blink_a", commit_id1=None, commit_id2=None):
        # Create a temporary folder to store the main.elf files
        os.makedirs(self.main_path + "\\build\\compare", exist_ok=True)
        # Check out to a specific commit
        if commit_id1 is not None:
            self.git_checkout(commit_id1)
        else:
            # Checkout to the latest commit
            self.git_checkout()
        # Build the firmware with the first commit ID
        self.build(build_option)
        # Temporary replace main.elf to main1.elf
        if os.path.exists(self.main_path + "\\build\\compare\\main1.elf"):
            os.remove(self.main_path + "\\build\\compare\\main1.elf")
        os.rename(self.main_path + "\\build\\main.elf", self.main_path + "\\build\\compare\\main1.elf")
        # Check out to a specific commit
        if commit_id2 is not None:
            self.git_checkout(commit_id2)
        else:
            # Checkout the 2nd latest commit
            commit_id2 = os.popen('git log --pretty=format:"%H"').read().strip().splitlines()[1]
            self.git_checkout(commit_id2)
        # Build the firmware with the second commit ID
        self.build(build_option)
        # Temporary replace main.elf to main2.elf
        if os.path.exists(self.main_path + "\\build\\compare\\main2.elf"):
            os.remove(self.main_path + "\\build\\compare\\main2.elf")
        os.rename(self.main_path + "\\build\\main.elf", self.main_path + "\\build\\compare\\main2.elf")
        # Compare the two main.elf files
        print(f"Compare the two main.elf files of commit ID: {commit_id1} vs {commit_id2}")
        result = filecmp.cmp(f"{self.main_path}\\build\\compare\\main1.elf", f"{self.main_path}\\build\\compare\\main2.elf")
        # Remove the temporary folder
        subprocess.run(f"rmdir /s /q {self.main_path}\\build\\compare", shell=True)
        # Checkout back to master
        self.git_checkout()
        return result

    def clean(self):
        subprocess.run(["make clean"], cwd=self.main_path)

    def download(self, flash_option="flash_a"):
        subprocess.run("make " + flash_option, cwd=self.main_path)

    def auto_build(self, build_option="blink_a", no_commits=None):
        # Get the history of the GIT
        log_list = self.git_log(no_commits)
        # flash_option = "flash_" + build_option.split('_')[1]
        for log in log_list[::-1]:
            commit_id = log['id']
            self.git_checkout(commit_id)
            if not self.check_release(build_option, commit_id):
                print(f"Build and release the firmware with commit ID: {commit_id}")
                self.release(build_option, commit_id)
            else:
                print(f"Release folder with commit ID: {commit_id} already exists")

        # Checkout back to master
        self.git_checkout()

    def check_release(self, build_option="blink_a", commit_id=None):
        if commit_id is not None:
            commit_id = self.git_log(1)[0]['id']
        # Check the release folder with the build option and commit ID exist
        return os.path.exists(self.main_path + f"\\release\\{build_option}\\{build_option}_{commit_id}.zip")

    @staticmethod
    def git_info(print_info=False):
        # Get information from GIT
        info = os.popen('git log -1 --pretty=format:"%H%n%an%n%ae%n%ad%n%s"').read().strip().splitlines()
        info_dict = {'id': info[0], 'author': info[1], 'email': info[2], 'date': info[3], 'message': info[4]}
        # Print out git information
        if print_info:
            print("Git information -----------------------------------------")
            print(f"Commit ID   : {info_dict['id']}")
            print(f"Author      : {info_dict['author']}")
            print(f"Email       : {info_dict['email']}")
            print(f"Date        : {info_dict['date']}")
            print(f"Message     : {info_dict['message']}")
            print("---------------------------------------------------------")
        return info_dict

    @staticmethod
    def git_log(no_commits=None):
        # Get the history of the GIT
        #info = os.popen("git log --pretty=format:'%H %ct %<(20)%an %s' -10").read().strip().splitlines()   # Print out the history of the GIT
        if no_commits is None:
            log = os.popen('git log --pretty=format:"%H %ct %<(20)%an %s"').read().strip().splitlines()
        else:
            log = os.popen(f'git log --pretty=format:"%H %ct %<(20)%an %s" -{no_commits}').read().strip().splitlines()
        # Result is a list of dictionaries
        log_list = []
        for line in log:
            commit_id, timestamp, temp = line.split(' ', 2)
            author = temp[0:20].strip()
            message = temp[20:].strip()
            log_list.append({'id': commit_id, 'timestamp': timestamp, 'author': author, 'message': message})
        return log_list

    def git_checkout(self, commit_id=None):
        # Checkout to a specific commit
        # If commit_id is not provided, checkout latest commit
        if commit_id:
            subprocess.run("git checkout " + commit_id, cwd=self.main_path)
        else:
            subprocess.run("git checkout master", cwd=self.main_path)

    @staticmethod
    def isint(value):
        try:
            int(value)
            return True
        except ValueError:
            return False

    @staticmethod
    def ishex(value):
        try:
            int(value, 16)
            return True
        except ValueError:
            return False

# Test the class
if __name__ == "__main__":
    firmware = Firmware()
    firmware.git_info(print_info=True)
    firmware.git_checkout(commit_id=None)   # Checkout to the latest commit
    # Compare same latest commits
    print("Binary compare result: ", firmware.compare("blink_a"))
    firmware.build("blink_a")
    # firmware.download("flash_c")
    firmware.release("blink_a")
    firmware.readmacro()
    firmware.auto_build('blink_c', 2)
    print("Firmware test passed")