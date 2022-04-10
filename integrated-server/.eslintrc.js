module.exports = {
    'root': true,
    'env': {
        'browser': false,
        'node': true,
        'es2021': true
    },
    'extends': [
        'eslint:recommended',
        'plugin:@typescript-eslint/recommended'
    ],
    'parser': '@typescript-eslint/parser',
    'parserOptions': {
        'ecmaVersion': 'latest',
        'sourceType': 'module'
    },
    'plugins': [
        '@typescript-eslint'
    ],
    'parser': '@typescript-eslint/parser',
    'rules': {
        'indent': [
            'error',
            2
        ],
        'linebreak-style': [
            'error',
            'unix'
        ],
        'quotes': [
            'error',
            'single'
        ],
        'semi': [
            'error',
            'always'
        ],
        'no-irregular-whitespace': [
            'error'
        ],
        'space-before-blocks':[
            'error',
        ],
        'space-before-function-paren':[
            'error'
        ],
        'space-infix-ops':[
            'error'
        ],
        'space-unary-ops':[
            'error'
        ],
        'arrow-spacing':[
            'error'
        ]
    }
};
