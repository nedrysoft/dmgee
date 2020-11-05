import dmgbuild
import os

if __name__ == '__main__':
    dmgFilename = os.path.join(os.path.join(os.path.expanduser('~')), 'Desktop/Test.dmg')

    dmgbuild.build_dmg(volume_name='Regular Expressions 101',
                       filename=dmgFilename,
                       settings_file='./dmgconf.py',
                       settings={},
                       defines={
                           'sourceRoot':  '/Users/adriancarpenter/Documents/Development/regex101/',
                           'buildArch': 'x86_64'
                       },
                       lookForHiDPI=True,
                       detach_retries=5
                       )
