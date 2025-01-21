import os
import subprocess

class Firmware(object):
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
        subprocess.run(["make ", build_option], cwd=self.main_path)

    def clean(self):
        subprocess.run(["make clean"], cwd=self.main_path)

    def download(self, flash_option="flash_a"):
        subprocess.run(["make ", flash_option], cwd=self.main_path)

    @staticmethod
    def git_info(print_info=False):
        # Get information from GIT
        info = os.popen('git log -1 --pretty=format:"%H%n%an%n%ae%n%ad%n%s"').read().strip().splitlines()
        info_dict = {}
        info_dict['id']         = info[0]
        info_dict['author']     = info[1]
        info_dict['email']      = info[2]
        info_dict['date']       = info[3]
        info_dict['message']    = info[4]
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

    def git_checkout(self, commit_id=None):
        # Checkout to a specific commit
        # If commit_id is not provided, checkout latest commit
        if commit_id:
            subprocess.run(["git checkout ", commit_id], cwd=self.main_path)
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
    firmware.build("blink_a")
    firmware.download("flash_a")
    firmware.readmacro()
    print("Firmware test passed")